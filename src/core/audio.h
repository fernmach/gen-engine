/**
 *  \file audio.h
 *  \brief Audio subsystem
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * This unit provides wrapers arround audio direver methods.
 * It is responsable to manage all audio assets, its registration, playback and events.
 * No imediate play back is executed. The audio and it's property is published to an audio 
 * event queue, and consumed and executed later by the audio syssystem 
 */

#ifndef _ENG_AUDIO_H_
#define _ENG_AUDIO_H_

#include "config.h"
#include "audio_defines.h"
#include "audio_types.h"

#include <genesis.h>

// Forward declare audio event buss
static CircularBuffer g_audio_event_bus;

void Audio_init();
AudioId Audio_addSource(Audio audio);
void Audio_removeSource(AudioId audioId);
void Audio_clearAllSources();
void Audio_shutdown();

void Audio_play(AudioId id);
void Audio_pause(AudioId id);
void Audio_resume(AudioId id);
void Audio_stop(AudioId id);

// void Audio_fadeIn();
// void Audio_fadeOut();

void Audio_processEvents();

static inline void Audio_raiseEvent(AudioId id, AudioEventType eventType) {
    AudioEvent event = {
        .type = eventType,
        .audioId = id
    };

    if ( !CB_write( &g_audio_event_bus, &event ) ) {
        LOGGER_WARN("Audio Event: buffer full. Dropped source play %d", id);        
    }

    //SOUND_DRIVER_PLAY(source);    
    // Try to add a "shoot" event.
    LOGGER_DEBUG("Audio buffer: Size: %d, Cap: %d, Count: %d",
        AUDIO_BUFFER_CAPACITY, g_audio_event_bus.capacity,
        g_audio_event_bus.count
    );
}

//-- Debuging utility functions
static inline SOUND_DRIVER_CPU_LOAD_TYPE Audio_getCPULoad() {
    return SOUND_DRIVER_GET_CPU_LOAD;
}

static inline void Audio_showCPULoad(u16 x, u16 y) {
    char audio_load_indicator[4];
    sprintf(audio_load_indicator, SOUND_DRIVER_CPU_LOAD_FORMAT, Audio_getCPULoad() );
    VDP_clearText(x, y, 4);
    VDP_drawText(audio_load_indicator, x, y);
}

#endif //_ENG_AUDIO_H_
