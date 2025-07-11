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

/*

#ifndef _ENG_AUDIO_H_
#define _ENG_AUDIO_H_

#include "config.h"
#include "audio_defines.h"
#include "audio_types.h"
#include "music.h"
#include "sfx.h"
#include "containers/audio_buffer.h"

#include <genesis.h>

// Forward declare audio event buss
static AudioBuffer g_audio_event_bus;
bool AB_write(AudioBuffer* cb, AudioEvent event);
bool AB_read(AudioBuffer* cb, AudioEvent* event_out);

/*void Audio_init();
void Audio_clearBuffer();
void Audio_shutdown();*/

/*
//-- Debuging utility functions
static inline AUDIO_DRIVER_CPU_LOAD_TYPE Audio_getCPULoad() {
    return AUDIO_DRIVER_GET_CPU_LOAD();
}

static inline void Audio_showCPULoad(u16 x, u16 y) {
    char audio_load_indicator[4];
    sprintf(audio_load_indicator, AUDIO_DRIVER_CPU_LOAD_FORMAT, AUDIO_DRIVER_GET_CPU_LOAD() );
    VDP_clearText(x, y, 4);
    VDP_drawText(audio_load_indicator, x, y);
}

static inline void Audio_dispatch(AudioEvent event) {
    if ( !AB_write( &g_audio_event_bus, event ) ) {
        LOGGER_WARN("Audio Event: buffer full. Dropped source play %s", event.name);
    }

    //SOUND_DRIVER_PLAY(source);    
    // Try to add a "shoot" event.
    LOGGER_DEBUG("Audio buffer: Size: %d, Cap: %d, Count: %d",
        AUDIO_BUFFER_CAPACITY, g_audio_event_bus.capacity,
        g_audio_event_bus.count
    );
}

static inline void Audio_processEvents() {
    AudioEvent event;

    // Use a 'while' loop, NOT an 'if'.
    // This ensures we process all pending events in a single frame if needed,
    // preventing a backlog.
    while (AB_read(&g_audio_event_bus, &event)) {
        
        switch (event.type) {                        
            case AUDIO_EVENT_PLAY_SFX:
                event.data.sfx.properties = SFX_DEFAULT_CONFIG;                
                SFX_play(&(event.data.sfx));
                break;

            case AUDIO_EVENT_PLAY_SFX_WITH_PROPERTIES:                
                SFX_play(&(event.data.sfx));
                break;

            default:
                LOGGER_WARN("Audio: Failed to process event. Unknon type %d", event.type);
                break;
        }
        
    } 
}
    
#endif //_ENG_AUDIO_H_

*/