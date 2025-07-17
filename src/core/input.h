// input.h
#ifndef _ENG_INPUT_H_
#define _ENG_INPUT_H_

#include "config.h"
//#include <genesis.h>

// SGDK Forward declaration
typedef unsigned char u8;
typedef u8 bool;
typedef unsigned short u16;

typedef struct {
    u16 current;
    u16 previous;
} JoypadState;

extern JoypadState g_joypad1;

// Input variable declaration
#if (INPUT_MAX_JOYPADS == JOY_2)
    extern JoypadState g_joypad2;
#elif (INPUT_MAX_JOYPADS == JOY_3)
    extern JoypadState g_joypad2;
    extern JoypadState g_joypad3;
#elif (INPUT_MAX_JOYPADS == JOY_4)
    extern JoypadState g_joypad2;
    extern JoypadState g_joypad3;
    extern JoypadState g_joypad4;
#endif

void Input_init();
void Input_update();
void Input_shutdown();

void Input_joyEventCallback(u16 joy, u16 changed, u16 state);

// Helper functions
bool Input_isPressed(u8 joyId, u16 button_mask);      // Button is currently down
bool Input_isJustPressed(u8 joyId, u16 button_mask);  // Button was just pressed this frame
bool Input_isReleased(u8 joyId, u16 button_mask);     // Button was just released this frame

#define REGISTER_JOY_INPUT_ACCESSORS(number) \
static inline bool Input_isPressed_JOY_##number(u16 button_mask) { \
    return (g_joypad##number.current & button_mask); \
} \
static inline bool Input_isJustPressed_JOY_##number(u16 button_mask) { \
    return (g_joypad##number.current & button_mask) && !(g_joypad##number.previous & button_mask); \
} \
static inline bool Input_isReleased_JOY_##number(u16 button_mask) { \
    return !(g_joypad##number.current & button_mask) && (g_joypad##number.previous & button_mask); \
} \
static inline bool Input_areAllPressed_JOY_##number(u16 button_mask) { \
    return (g_joypad##number.current & button_mask) == button_mask; \
} \
static inline bool Input_areAllJustPressed_JOY_##number(u16 button_mask) { \
    return ( ((g_joypad##number.current & button_mask) == button_mask) && \
        ((g_joypad##number.previous & button_mask) != button_mask) );  \
} \
static inline bool Input_areAllJustReleased_JOY_##number(u16 button_mask) { \
   return ( ((g_joypad##number.current & button_mask) != button_mask) && \
        ((g_joypad##number.previous & button_mask) == button_mask) );  \
} \
static inline bool Input_isExactPress_JOY_##number(u16 button_mask) { \
    return (g_joypad##number.current == button_mask); \
}

// Register individual joy input acessors;
REGISTER_JOY_INPUT_ACCESSORS(1);

#if (INPUT_MAX_JOYPADS == JOY_2)
    REGISTER_JOY_INPUT_ACCESSORS(2);
#elif (INPUT_MAX_JOYPADS == JOY_3)
    REGISTER_JOY_INPUT_ACCESSORS(2);
    REGISTER_JOY_INPUT_ACCESSORS(2);
#elif (INPUT_MAX_JOYPADS == JOY_4)
    REGISTER_JOY_INPUT_ACCESSORS(2);
    REGISTER_JOY_INPUT_ACCESSORS(3);
    REGISTER_JOY_INPUT_ACCESSORS(4);
#endif

// Define an JOY_X generic acessor fore easier usage
#define INPUT_IS_PRESSED(JOY_ID, button_mask) \
    Input_isPressed_##JOY_ID(button_mask)

#define INPUT_IS_JUST_PRESSED(JOY_ID, button_mask) \
    Input_isJustPressed_JOY_##number(button_mask)

#define INPUT_IS_RELEASED(JOY_ID, button_mask) \
    Input_isReleased_JOY_##number(button_mask)

#define INPUT_ARE_ALL_PRESSED(JOY_ID, button_mask) \
    Input_areAllPressed_##JOY_ID(button_mask)

#define INPUT_ARE_ALL_JUST_PRESSED(JOY_ID, button_mask) \
    Input_areAllJustPressed_##JOY_ID(button_mask)

#define INPUT_ARE_ALL_JUST_RELEASED(JOY_ID, button_mask) \
    Input_areAllJustReleased_##JOY_ID(button_mask)

#define INPUT_IS_EXACT_PRESS(JOY_ID, button_mask) \
    Input_isExactPress_JOY_##JOY_ID(button_mask) \

#endif // _ENG_INPUT_H_