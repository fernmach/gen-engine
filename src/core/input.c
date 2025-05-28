#include "logger.h"
#include "input.h"
#include <genesis.h>

//BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_START.

JoypadState g_joypad1;

#if INPUT_JOYPAD_2_ENABLED
    JoypadState g_joypad2;
#endif

// SGDK joy event callback. Registered on input init.
// TODO: Is it better to use this event to get all button states in one go?
void Input_joyEventCallback(u16 joy, u16 changed, u16 state) {
    LOGGER_DEBUG("Input: Joy %d event fired", joy);    
}

void Input_init() {

    // Initialize SGDK joy sybsystem
    JOY_init();
    LOGGER_INFO("Input: Initializing SGDK joy subsystem: JOY_Init().");

    // Register sgdk's vblank event handler
    JOY_setEventHandler( &Input_joyEventCallback );
    LOGGER_INFO("Input: Registering SGDK joy event handler.");

    g_joypad1.current = 0;
    g_joypad1.previous = 0;    
    // JOY_setEventHandler( myJoyHandler ); // Optional: for interrupt-driven input

    #if INPUT_JOYPAD_2_ENABLED
        LOGGER_INFO("Input: Joypad 2 enabled.");
        g_joypad2.current = 0;
        g_joypad2.previous = 0;    
    #endif

    LOGGER_INFO("Input subsystem initialized.");
}

void Input_update() {
    // Player 1
    g_joypad1.previous = g_joypad1.current;
    g_joypad1.current = JOY_readJoypad(JOY_1);

    // For player 2
    #if INPUT_JOYPAD_2_ENABLED
        g_joypad2.previous = g_joypad2.current;
        g_joypad2.current = JOY_readJoypad(JOY_2);
    #endif
}

void Input_shutdown() {    
    LOGGER_INFO("Input subsystem shutting down.");
}

// TODO: Include methods for other joypads
bool Input_isPressed(u16 button_mask) {    
    return (g_joypad1.current & button_mask);
}

bool Input_isJustPressed(u16 button_mask) {
    return (g_joypad1.current & button_mask) && !(g_joypad1.previous & button_mask);
}

bool Input_isReleased(u16 button_mask) {
    return !(g_joypad1.current & button_mask) && (g_joypad1.previous & button_mask);
}