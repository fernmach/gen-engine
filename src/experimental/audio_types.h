/*
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
    AUDIO_EVENT_RESUME_MUSIC_WITH_PROPERTIES,
    AUDIO_EVENT_PLAY_SFX,
    AUDIO_EVENT_PLAY_SFX_WITH_PROPERTIES,
    AUDIO_EVENT_STOP_SFX,
    
    AUDIO_MAX_EVENT_TYPES
} AudioEventType;

typedef enum {
    SFX_CHANNEL_AUTO = -1,
    SFX_CHANNEL_1 = 0,
    SFX_CHANNEL_2,
    SFX_CHANNEL_3,
    SFX_CHANNEL_4,
    SFX_CHANNEL_MAX
} SFXChannel;

typedef enum {
    SFX_CHANNEL1_MSK = 1 << SOUND_PCM_CH1,
    SFX_CHANNEL2_MSK = 1 << SOUND_PCM_CH2,
    SFX_CHANNEL3_MSK = 1 << SOUND_PCM_CH3,
    SFX_CHANNEL4_MSK = 1 << SOUND_PCM_CH4
} SFXChannelMask;

// Audio sample definition
typedef struct {
    u8* data;
    u32 len;
} Source;

typedef struct {    
    u8 channel;
    u8 priority;
} SFXProperties;

typedef struct {    
    const s8 loop;
    const u16 tempo;
    const u16 volume;
} MusicProperties;

typedef struct {
    Source audio;
    SFXProperties properties;
} SFX;

typedef struct {
    Source audio;
    MusicProperties properties;
} Music;

typedef struct {
    union{
        //MusicProperties musicProperties;        
        Source audio;
        SFX sfx;
        Music music;
        u16 volume;
        u16 tempo;
        u8 loop;
        u8 channel;
        //u8 priority
    } data;
    char* name; // for debugging
    AudioEventType type;
} AudioEvent;



#endif // _ENG_AUDIO_TYPES_H_
*/