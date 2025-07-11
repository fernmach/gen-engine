#pragma once
#ifndef _ENG_AUDIO_TYPES_H_
#define _ENG_AUDIO_TYPES_H_

#include <genesis.h>

// Define the audio events your game will have.
// Using an enum makes the code much more readable.
typedef enum {
    AUDIO_EVENT_NONE = 0, // Good practice to have a 'none' event

    AUDIO_EVENT_PLAY_MUSIC,
    AUDIO_EVENT_PAUSE_MUSIC,
    AUDIO_EVENT_STOP_MUSIC,
    AUDIO_EVENT_RESUME_MUSIC,
    AUDIO_EVENT_SET_MUSIC_TEMPO,    

    AUDIO_EVENT_PLAY_SFX,
    AUDIO_EVENT_PLAY_SFX_EX,
    AUDIO_EVENT_STOP_SFX,
    
    AUDIO_MAX_EVENT_TYPES
} AudioEventType;

// Audio sample definition
typedef struct {
    u32 len;
    const u8* sample;    
} Source;

typedef struct {    
    //u8 channel should also work
    SoundPCMChannel channel;
    u8 priority;    
    bool halfRate;
    bool loop;
} SFXProperties;

typedef struct {
    Source source;
    SFXProperties properties;
} SFX;

typedef struct {
    union {           
        SFX sfx;
        Source source;        
        u16 volume;
        u16 tempo;
        u8 loop;
        u8 channel;
    } data;
    char* name; // for debugging
    AudioEventType type;
} AudioEvent;

#endif //_ENG_AUDIO_TYPES_H_