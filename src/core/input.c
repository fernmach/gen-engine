#include "logger.h"
#include "asserts.h"
#include "input.h"
#include <genesis.h>

//BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_START.

JoypadState g_joypad1;

#define INPUT_INITIALIZE(number) \
    g_joypad##number.current = 0; \
    g_joypad##number.previous = 0;

// Input initialization helper
#define INPUT_VARIABLE_INITIALIZATION(number) \
    INPUT_INITIALIZE(number) \
    LOGGER_INFO("Input: Joypad "#number" enabled.");

// Input update helper
#define INPUT_UPDATE(number) \
    g_joypad##number.previous = g_joypad##number.current; \
    g_joypad##number.current = JOY_readJoypad(JOY_##number);

// Input variable declaration
#if (INPUT_MAX_JOYPADS == JOY_2)
    JoypadState g_joypad2;
#elif (INPUT_MAX_JOYPADS == JOY_3)
    JoypadState g_joypad2;
    JoypadState g_joypad3;
#elif (INPUT_MAX_JOYPADS == JOY_4)
    JoypadState g_joypad2;
    JoypadState g_joypad3;
    JoypadState g_joypad4;
#endif


// SGDK joy event callback. Registered on input init.
// TODO: Is it better to use this event to get all button states in one go?
// void Input_joyEventCallback(u16 joy, u16 changed, u16 state) {
//     LOGGER_DEBUG("Input: Joy %d event fired", joy);    
// }

void Input_init() {

    // Initialize SGDK joy sybsystem
    // This part of the code is only needed if the eventhandler is set here.
    // Otherwise the engine will take care of the initialization
    // JOY_init();
    // LOGGER_INFO("Input: Initializing SGDK joy subsystem: JOY_Init().");

    // Register sgdk's vblank event handler
    //JOY_setEventHandler( &Input_joyEventCallback );
    //LOGGER_INFO("Input: Registering SGDK joy event handler.");

    g_joypad1.current = 0;
    g_joypad1.previous = 0;    
    // JOY_setEventHandler( myJoyHandler ); // Optional: for interrupt-driven input

    #if INPUT_MAX_JOYPADS == JOY_2
       INPUT_VARIABLE_INITIALIZATION(2);
    #elif INPUT_MAX_JOYPADS == JOY_3
        INPUT_VARIABLE_INITIALIZATION(2);
        INPUT_VARIABLE_INITIALIZATION(3);
    #elif INPUT_MAX_JOYPADS == JOY_4
        INPUT_VARIABLE_INITIALIZATION(2);
        INPUT_VARIABLE_INITIALIZATION(3);
        INPUT_VARIABLE_INITIALIZATION(4);
    #endif

    LOGGER_INFO("Input subsystem initialized.");
}

void Input_update() {

    // Update the joycon reading state values
    JOY_update();

    // Player 1
    g_joypad1.previous = g_joypad1.current;
    g_joypad1.current = JOY_readJoypad(JOY_1);

    #if INPUT_MAX_JOYPADS == JOY_2        
       INPUT_UPDATE(2);
    #elif INPUT_MAX_JOYPADS == JOY_3
        INPUT_UPDATE(2);
        INPUT_UPDATE(3);
    #elif INPUT_MAX_JOYPADS == JOY_4
        INPUT_UPDATE(2);
        INPUT_UPDATE(3);
        INPUT_UPDATE(4);
    #endif
}

void Input_shutdown() {
    g_joypad1.current = 0;
    g_joypad1.previous = 0;    
    
    #if INPUT_MAX_JOYPADS == JOY_2        
       INPUT_INITIALIZE(2);
    #elif INPUT_MAX_JOYPADS == JOY_3
        INPUT_INITIALIZE(2);
        INPUT_INITIALIZE(3);
    #elif INPUT_MAX_JOYPADS == JOY_4
        INPUT_INITIALIZE(2);
        INPUT_INITIALIZE(3);
        INPUT_INITIALIZE(4);
    #endif

    LOGGER_INFO("Input subsystem shutting down.");
}

// FIXME: Deprecated. Remove in favor of the static input access
bool Input_isPressed(u8 joyId, u16 button_mask) {    
    ASSERT_MSG(joyId<=INPUT_MAX_JOYPADS, "Invalid joypad id on Input_isPressed");

    switch (joyId)
    {
        case JOY_1:
            return (g_joypad1.current & button_mask);    
            break;

        #if INPUT_MAX_JOYPADS == JOY_2
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

        #if INPUT_MAX_JOYPADS == JOY_2
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

        #if INPUT_MAX_JOYPADS == JOY_2
        case JOY_2:
            return !(g_joypad2.current & button_mask) && (g_joypad2.previous & button_mask);
            break;
        #endif
    }

    return FALSE;
}