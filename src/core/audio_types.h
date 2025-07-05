#ifndef _ENG_AUDIO_TYPES_H_
#define _ENG_AUDIO_TYPES_H_

#include "config.h"
#include "logger.h"
#include "asserts.h"
#include "audio_defines.h"
#include "containers/circular_buffer.h"

#include <genesis.h>

// Defome audio component current state
typedef enum {
    AUDIO_STATE_PLAYING,
    AUDIO_STATE_STOPED,
    AUDIO_STATE_PAUSED
} AudioState;

// Defome audio component current state
typedef enum {
    AUDIO_TYPE_MUSIC,
    AUDIO_TYPE_SFX
} AudioType;

// Define an audio component
typedef struct
{   
    const u8* source;
    u32 size;
    AudioType type;
    u8 channel;
    u8 priority;
} Audio;

// Define an audio component
typedef struct
{
    const u8* sample;
    u32 size;
} MusicAudio;

// SFX audio definition, as in defined in the sfx
typedef SFXData SFXAudio;

// Define the audio events your game will have.
// Using an enum makes the code much more readable.
typedef enum {
    AUDIO_EVENT_NONE = 0, // Good practice to have a 'none' event
    AUDIO_EVENT_PLAY,
    AUDIO_EVENT_PAUSE,
    AUDIO_EVENT_STOP,
    AUDIO_EVENT_RESUME,
    AUDIO_EVENT_UPDATE, // For volume, tempo, loop and others.
    AUDIO_MAX_EVENT_TYPES
} AudioEventType;

typedef struct {
    AudioId audioId;
    AudioEventType type;
} AudioEvent;

//XGM_startPlay()  Play the music
//XGM_stopPlay()
//XGM_resumePlay()
//XGM_setMusicTempo() set the tempo
//XGM_setLoopNumber set the loop number (0 equal single play)
//XGM_isPlaying
//loop
//tempo


#endif // _ENG_AUDIO_TYPES_H_