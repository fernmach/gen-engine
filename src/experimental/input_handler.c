#include "../core/config.h"
#include "../core/logger.h"

#include "../experimental/input_handler.h"

// In your input header file (e.g., input_handler.h)
#define INPUT_BUFFER_SIZE 60 // Frames (1 second at 60Hz)
#define MAX_REGISTERED_COMMANDS 32 // Adjust as needed

// --- Global variables (in input_handler.c) ---
static JoypadState g_input_buffer_p1[INPUT_BUFFER_SIZE];
static u16 g_current_tick_p1 = 0;

// For detecting pressed/released edges
static JoypadState g_current_joy_state_p1 = 0;
static JoypadState g_prev_joy_state_p1 = 0;
static JoypadState g_pressed_edge_p1 = 0;
static JoypadState g_released_edge_p1 = 0;

// --- hangling commands ---
static RegisteredCommand g_registered_commands[MAX_REGISTERED_COMMANDS];
static u8 g_num_registered_commands = 0;

void InputHandler_init(void) {
    memset(g_input_buffer_p1, 0, sizeof(g_input_buffer_p1));

    g_current_tick_p1 = 0;    
    g_current_joy_state_p1 = 0;
    g_prev_joy_state_p1 = 0;    
    g_num_registered_commands = 0;

    LOGGER_INFO("InputHandler subsystem initialized.");
}

// Call this once per game frame, very early in your main loop
void InputHandler_update() {

    g_prev_joy_state_p1 = g_current_joy_state_p1;
    g_current_joy_state_p1 = JOY_readJoypad(JOY_1);
    g_pressed_edge_p1 = (g_current_joy_state_p1 & ~g_prev_joy_state_p1);
    g_released_edge_p1 = (~g_current_joy_state_p1 & g_prev_joy_state_p1);

    g_input_buffer_p1[g_current_tick_p1] = g_current_joy_state_p1;
    g_current_tick_p1 = (g_current_tick_p1 + 1) % INPUT_BUFFER_SIZE;

    // --- Process Registered Commands ---
    // Optional: Sort commands by priority if not already registered in that order.
    // For simplicity, assume they are registered from highest to lowest priority.

    // To prevent multiple commands consuming same input
    bool command_fired_this_frame_p1 = FALSE;

    for (u8 i = 0; i < g_num_registered_commands; ++i) {
        RegisteredCommand* cmd = &g_registered_commands[i];
        bool matched = FALSE;

        // Detectect what is the correct player
        // Target the correct player's state
        
        JoypadState current_joy = 0;
        JoypadState pressed_edge = 0;
        JoypadState released_edge = 0;
        bool* command_fired_flag;

        switch (cmd->player_idx)
        {
            case JOY_1:
                current_joy = g_current_joy_state_p1;
                pressed_edge = g_pressed_edge_p1;
                released_edge = g_released_edge_p1;
                command_fired_flag = &command_fired_this_frame_p1;
                break;
            
            default:
                break;
        }
        
        //if (command_fired_this_frame_p1 && cmd->type == INPUT_TYPE_SEQUENCE) {
             // Or based on cmd->consume_inputs
             // A higher priority sequence command already fired for this player
             // and potentially consumed the inputs.
             // Simple single presses might still be allowed.
             // This logic needs careful thought based on game feel.
             // For now, let's assume a sequence consumes, but single presses don't prevent others.
        //}

        switch (cmd->type) {
            case INPUT_TYPE_SINGLE_PRESS:
                if (pressed_edge & cmd->button_mask) {
                    matched = TRUE;
                }
                break;
            case INPUT_TYPE_SINGLE_RELEASE:
                if (released_edge & cmd->button_mask) {
                    matched = TRUE;
                }
                break;
            case INPUT_TYPE_HELD: // Often better handled by direct polling in game logic
                if (current_joy & cmd->button_mask) {
                    matched = TRUE;
                }
                break;
            case INPUT_TYPE_SEQUENCE:
                // Only check sequences if a higher priority one hasn't fired for this player
                if (!(*command_fired_flag)) {
                        if (InputHandler_checkSequence(
                                cmd->player_idx, 
                                cmd->sequence, 
                                cmd->sequence_length, 
                                cmd->max_duration_frames
                            )
                        ) {
                            matched = TRUE;
                            *command_fired_flag = TRUE; // Mark that a sequence fired
                        }
                }
                break;
        }

        if (matched) {
            // EVENT_Raise(cmd->command_id, (void*)cmd->player_idx); // Your event system
            // For example:
            // KDebug_AlertNumber(cmd->command_id); // Placeholder
            char buf[64];
            sprintf(buf, "Cmd: %s (ID %d) P%d", cmd->name, cmd->command_id, cmd->player_idx + 1);
            VDP_drawText(buf, 0, 20 + i); // Example output
        }

    }
}

// Adapted CheckSequence
bool InputHandler_checkSequence(u8 player_idx, const JoypadState* sequence, u8 sequence_length, u8 max_duration_frames) {
    if (sequence_length == 0) return FALSE;

    JoypadState* buffer_to_check = g_input_buffer_p1;
    u16 current_tick_offset = g_current_tick_p1;

    //s8 w = sequence_length - 1; // Start with the last element of the command sequence
    s16 w = sequence_length - 1; // Start with the last element of the command sequence

    // Iterate backwards through the input buffer from the current (most recent) input
    // up to max_duration_frames ago.
    for (s16 i = 0; i < max_duration_frames; ++i) {
        // + INPUT_BUFFER_SIZE is to handle potential negative results before modulo
        JoypadState buffer_state = buffer_to_check[(current_tick_offset - 1 - i + INPUT_BUFFER_SIZE) % INPUT_BUFFER_SIZE];

        // Check if the current state in the buffer satisfies the current sequence step.
        // This means ALL bits set in sequence[w] must also be set in buffer_state.
        // Other bits in buffer_state can be set (e.g. player pressing A+B when sequence only needs A).
        if ((buffer_state & sequence[w]) == sequence[w]) {
            w--; // Matched this part of the sequence, look for the previous part
            if (w < 0) {
                // KDebug_Alert("Sequence Matched!"); // Debug
                return TRUE; // Entire sequence matched
            }
        }
        // Optional: Add max_delay_between_steps logic here.
        // If sequence[w+1] was matched at frame F1, and sequence[w] is matched at F2,
        // then (F1 - F2) must be <= max_delay_between_steps.
        // This requires storing the frame index of the last match.
    }
    return FALSE; // Sequence not found within max_duration
}

void InputHandler_registerCommand(const RegisteredCommand* command_template) {
    if (g_num_registered_commands < MAX_REGISTERED_COMMANDS) {
        g_registered_commands[g_num_registered_commands] = *command_template; // Shallow copy, sequence pointer must remain valid!
        // If sequence data is dynamically allocated or temporary, deep copy here.
        // For const global arrays, shallow copy is fine.
        g_num_registered_commands++;
        // Consider sorting by priority after adding if not added in order
    } else {
        // KDebug_Alert("Max registered commands reached!");
    }
}

// --- Helper functions for direct polling (optional, if not using events for everything) ---
//TODO: Make better helper functions
bool InputHandler_isPressed(u8 joy_idx, JoypadState button_mask) {
    if (joy_idx == JOY_1) {        
        return (g_current_joy_state_p1 & button_mask) == button_mask;
    }
    return FALSE;
}

bool InputHandler_isJustPressed(u8 joy_idx, JoypadState button_mask) {
    JoypadState pressed_edge = g_pressed_edge_p1;
    return (pressed_edge & button_mask) == button_mask;
}

bool InputHandler_isJustReleased(u8 joy_idx, JoypadState button_mask) {
    JoypadState released_edge = g_released_edge_p1;
    return (released_edge & button_mask) == button_mask;
}