// // input.h
// #ifndef _JOYPAD_H_
// #define _JOYPAD_H_

// #include <genesis.h>

// // In your input header file (e.g., input_handler.h)
// #define INPUT_BUFFER_SIZE 60 // Frames (1 second at 60Hz)

// //SGDK u16. This will be the fundamental piece of data we store.
// typedef u16 JoypadState;

// //Player one, two.... eg: JOY_1, JOY_2
// typedef u8 JoypadId;         

// // #define 	JOY_1   0x0000 
// // #define 	JOY_2   0x0001 
// // #define 	JOY_3   0x0002 
// // #define 	JOY_4   0x0003 
// // #define 	JOY_5   0x0004 
// // #define 	JOY_6   0x0005 
// // #define 	JOY_7   0x0006 
// // #define 	JOY_8   0x0007 
// // #define 	JOY_NUM   0x0008 
// // #define 	JOY_ALL   0xFFFF

// // Stores the state of all buttons for a single frame
// typedef struct {    
//     JoypadId joypadId;
//     JoypadState current;
//     JoypadState previous;
// } Joypad;

// Joypad* Joypad_init(JoypadId joypadId);

// #endif // _JOYPAD_H_
