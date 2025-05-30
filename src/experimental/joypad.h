// input.h
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "../core/config.h"
#include <genesis.h>

//SGDK u16. This will be the fundamental piece of data we store.
typedef u16 JoypadState;

//Player one, two.... eg: JOY_1, JOY_2
typedef u8 JoypadId;         

// #define 	JOY_1   0x0000 
// #define 	JOY_2   0x0001 
// #define 	JOY_3   0x0002 
// #define 	JOY_4   0x0003 
// #define 	JOY_5   0x0004 
// #define 	JOY_6   0x0005 
// #define 	JOY_7   0x0006 
// #define 	JOY_8   0x0007 
// #define 	JOY_NUM   0x0008 
// #define 	JOY_ALL   0xFFFF

// Stores the state of all buttons for a single frame
typedef struct {    
    JoypadId joypadId;
    JoypadState currentState;
    JoypadState previousState;
} Joypad;

extern Joypad g_joypads[INPUT_MAX_JOYPADS+1];

void Joypad_init();
void Joypad_update();

bool Joypad_isPressed(JoypadId joypadId, u16 button_mask);      // Button is currently down
bool Joypad_isJustPressed(JoypadId joypadId, u16 button_mask);  // Button was just pressed this frame
bool Joypad_isReleased(JoypadId joypadId, u16 button_mask);     // Button was just released this frame

// Helper functions
#define JOY1_isPressed(button_mask) \
    Joypad_isPressed(JOY_1, button_mask);

#define JOY1_isJustPressed(button_mask) \
    Joypad_isJustPressed(JOY_1, button_mask);

#define JOY1_isReleased(button_mask) \
    Joypad_isReleased(JOY_1, button_mask);

#endif // _JOYPAD_H_