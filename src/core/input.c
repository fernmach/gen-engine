#include "logger.h"
#include "asserts.h"
#include "input.h"
#include <genesis.h>

//BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_START.

JoypadState g_joypad1;

#if INPUT_MAX_JOYPADS == 2
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

    #if INPUT_MAX_JOYPADS == 2
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
    #if INPUT_MAX_JOYPADS == 2
        g_joypad2.previous = g_joypad2.current;
        g_joypad2.current = JOY_readJoypad(JOY_2);
    #endif
}

void Input_shutdown() {    
    LOGGER_INFO("Input subsystem shutting down.");
}

// TODO: Include methods for other joypads
bool Input_isPressed(u8 joyId, u16 button_mask) {    
    ASSERT_MSG(joyId<=INPUT_MAX_JOYPADS, "Invalid joypad id on Input_isPressed");

    switch (joyId)
    {
        case JOY_1:
            return (g_joypad1.current & button_mask);    
            break;

        #if INPUT_MAX_JOYPADS == 2
        case JOY_2:
            return (g_joypad2.current & button_mask);    
            break;
        #endif
    }

    return FALSE;    
}

bool Input_isJustPressed(u8 joyId, u16 button_mask) {
    ASSERT_MSG(joyId<=INPUT_MAX_JOYPADS, "Invalid joypad id on Input_isJustPressed");
    
    switch (joyId)
    {
        case JOY_1:
            return (g_joypad1.current & button_mask) && !(g_joypad1.previous & button_mask);
            break;

        #if INPUT_MAX_JOYPADS == 2
        case JOY_2:
            return (g_joypad2.current & button_mask) && !(g_joypad2.previous & button_mask); 
            break;
        #endif
    }

    return FALSE;    
}

bool Input_isReleased(u8 joyId, u16 button_mask) {
    ASSERT_MSG(joyId<=INPUT_MAX_JOYPADS, "Invalid joypad id on Input_isReleased");
    
    switch (joyId)
    {
        case JOY_1:
            return !(g_joypad1.current & button_mask) && (g_joypad1.previous & button_mask);
            break;

        #if INPUT_MAX_JOYPADS == 2
        case JOY_2:
            return !(g_joypad1.current & button_mask) && (g_joypad1.previous & button_mask);
            break;
        #endif
    }

    return FALSE;
}