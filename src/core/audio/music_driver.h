#ifndef _ENG_MUSIC_DRIVER_H_
#define _ENG_MUSIC_DRIVER_H_

#include <genesis.h>

// Pointer to an init function
typedef void (*PFN_onDriverInit)();
typedef void (*PFN_play)(const u8* song);
typedef void (*PFN_pause)();
typedef void (*PFN_resume)();
typedef void (*PFN_stop)();
typedef void (*PFN_setLoop)(const s8 value);
typedef void (*PFN_setTempo)(const u16 value);
typedef void (*PFN_setVolume)(const u16 value);
typedef bool (*PFN_isPlaying)();
typedef u16  (*PFN_getCPULoad)();
typedef void (*PFN_destroy)();
//typedef void (*PFN_onUpdate)();

// This struct IS the interface. It's a "contract" that any
// music implementation must fulfill by providing these functions.
typedef struct {
    // A name debugging or identification
    const char* name;

    // Pointer to an init function
    PFN_onDriverInit init;
     
    // Pointer to a play functions
    PFN_play play;
    PFN_pause pause; 
    PFN_resume resume;
    PFN_stop stop;

    // Function to set audio properties
    PFN_setLoop setLoop;
    PFN_setTempo setTempo;
    PFN_setVolume setVolume;
    PFN_isPlaying isPlaying;

    PFN_getCPULoad getCPULoad;

    PFN_destroy destroy;

    // Update function for polling(if needed)
    //PFN_onUpdate update;

} MusicDriver;

#endif //_ENG_MUSIC_DRIVER_H_