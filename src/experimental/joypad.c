#include "../experimental/joypad.h"

void Joypad_init() {
    memset(g_joypads, 0, sizeof(g_joypads));

    for(u8 i = 0; i <= INPUT_MAX_JOYPADS; i++) {
        g_joypads[i].joypadId = i;
        g_joypads[i].currentState = 0;
        g_joypads[i].previousState = 0;
    }
}

void Joypad_update() {
    for(u8 i = 0; i <= INPUT_MAX_JOYPADS; i++) {        
        g_joypads[i].previousState = g_joypads[i].currentState;
        g_joypads[i].currentState = JOY_readJoypad(i);
    }    
}

// Helper functions
bool Joypad_isPressed(JoypadId joypadId, u16 button_mask) {    
    return ( g_joypads[joypadId].currentState & button_mask);
}

bool Joypad_isJustPressed(JoypadId joypadId, u16 button_mask) {
    return (g_joypads[joypadId].currentState & button_mask) && 
        !(g_joypads[joypadId].previousState & button_mask);
}

bool Joypad_isReleased(JoypadId joypadId, u16 button_mask) {
    return !(g_joypads[joypadId].currentState & button_mask) &&
        (g_joypads[joypadId].previousState & button_mask);
}