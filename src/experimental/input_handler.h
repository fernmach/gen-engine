
// input.h
#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include "../core/config.h"
#include <genesis.h>

// In your input header file (e.g., input_handler.h)
//#define INPUT_BUFFER_SIZE 60 // Frames (1 second at 60Hz)
//#define MAX_REGISTERED_COMMANDS 32 // Adjust as needed

// Stores the state of all buttons for a single frame
typedef u16 JoypadState;

// In your input header file (e.g., input_handler.h)
typedef enum {
    INPUT_TYPE_SINGLE_PRESS,
    INPUT_TYPE_SINGLE_RELEASE,
    INPUT_TYPE_HELD, // Optional, can be checked directly
    INPUT_TYPE_SEQUENCE
} InputCommandType;

typedef struct {
    const char* name;             // For debugging
    u16 command_id;               // Unique ID for this command (to raise events)
    InputCommandType type;
    u8 player_idx;                // 0 for P1, 1 for P2

    // For INPUT_TYPE_SINGLE_PRESS, _RELEASE, _HELD
    JoypadState button_mask;      // e.g., BUTTON_A, BUTTON_START

    // For INPUT_TYPE_SEQUENCE
    const JoypadState* sequence;  // Array of button masks
    u8 sequence_length;
    u8 max_duration_frames;       // Max frames for the whole sequence
    // u8 max_delay_between_steps; // Optional: max frames between consecutive steps in sequence
    u8 priority;                  // Higher value = higher priority (checked first)

    // Could also add:
    bool consume_inputs;         // If true, these inputs won't trigger other lower-priority commands this frame
} RegisteredCommand;

void InputHandler_init();
void InputHandler_update();
void InputHandler_registerCommand(const RegisteredCommand* command_template);
bool InputHandler_isPressed(u8 joy_idx, JoypadState button_mask);
bool InputHandler_isJustPressed(u8 joy_idx, JoypadState button_mask);
bool InputHandler_isJustReleased(u8 joy_idx, JoypadState button_mask);
bool InputHandler_checkSequence(u8 player_idx, const JoypadState* sequence, u8 sequence_length, u8 max_duration_frames);

#endif //_INPUT_HANDLER_H_