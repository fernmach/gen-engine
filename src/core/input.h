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

#if INPUT_JOYPAD_2_ENABLED
    extern JoypadState g_joypad2; // if you need 2 player
#endif

void Input_init();
void Input_update();
void Input_shutdown();

void Input_joyEventCallback(u16 joy, u16 changed, u16 state);

// Helper functions
bool Input_isPressed(u8 joyId, u16 button_mask);      // Button is currently down
bool Input_isJustPressed(u8 joyId, u16 button_mask);  // Button was just pressed this frame
bool Input_isReleased(u8 joyId, u16 button_mask);     // Button was just released this frame

#endif // _ENG_INPUT_H_