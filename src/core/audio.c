#include "logger.h"
#include "audio.h"

#include "./containers/circular_buffer.h"

// This is the control structure for our buffer.
static CircularBuffer g_audio_event_bus;

// This is the actual memory for the audio event buffer.
static AudioEvent g_audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

// List of audio sources
static Audio g_audio_sources[AUDIO_MAX_SOURCES];

// Count of hou many audio sources are active(head of the array)
static AudioId g_audio_sources_count;

void Audio_init() {
    // Zero out all component data and entity states
    memset( g_audio_sources, 0, sizeof(g_audio_sources) );
    memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );   

    g_audio_sources_count = 0;

      // -- Initialize the VFX Event Bus --
    CB_init(&g_audio_event_bus, 
            (u8*)g_audio_event_buffer_data, // Cast the struct array to a byte pointer
            AUDIO_BUFFER_CAPACITY, 
            sizeof(AudioEvent)); // Tell the buffer the size of our struct

    // Identify if audio should be initialized here
    //SND_init();    

    // Important: Initialize SGDK Sprite engine before starting ECS (SPR_init())
    // This is implemente in the Engine_init() event
    //     //after initialization:    
    LOGGER_INFO("Audio subsystem initialized. Capacity %d", g_audio_event_bus.capacity);
}

AudioId Audio_addSource(Audio audio) {
    //TODO: Replace with assert
    if (g_audio_sources_count >= AUDIO_MAX_SOURCES) {
        LOGGER_WARN("Audio: Maximum number of sources reached %d", AUDIO_MAX_SOURCES);
        return AUDIO_MAX_SOURCES;
    }

    AudioId new_id = g_audio_sources_count;
    g_audio_sources[ new_id ] = audio;
    g_audio_sources_count++;

    LOGGER_DEBUG("Audio: Added source %d", g_audio_sources_count);    

    return new_id;
}

void Audio_removeSource(AudioId audioId) {
    // TODO: Identify how to stop specific source
    if( SOUND_DRIVER_IS_PLAYING() ) {
        SOUND_DRIVER_STOP();        
    }
    g_audio_sources[ audioId ].source = NULL;

    // Decrement the active entity count. "pop" the source
    g_audio_sources_count--;   

    // If the entity we are destroying is NOT the last one in the array,
    // we need to move the last one into its place to fill the gap.
    // Last Id  = g_audio_sources_count
    if (audioId != g_audio_sources_count) {
        g_audio_sources[ audioId ] = g_audio_sources[ g_audio_sources_count ];
    }

    LOGGER_DEBUG("Audio: Removed source %d ", audioId);
}

void Audio_clearAllSources() {
    if( SOUND_DRIVER_IS_PLAYING () ) {
        SOUND_DRIVER_STOP();        
    }

    // TODO: How to stop individual audio sources
    for (AudioId i = 0; i < g_audio_sources_count; ++i) {
        // Audio* audio = &g_audio_sources[i];
        // if( SOUND_DRIVER_IS_PLAYING (audio->source) ) {
        //     SOUND_DRIVER_STOP(audio->source);
            
        // }
        g_audio_sources[i].source = NULL;
    }

    //TODO: memset is slow evaluate necessit of doing it in real time
    //memset( g_audio_sources, 0, sizeof(g_audio_sources) );
    g_audio_sources_count = 0;

    //Celar the audio buss
    CB_clear(&g_audio_event_bus);

    LOGGER_INFO("Audio: Clearing all sources/buffer");
}

void Audio_shutdown() {
    //Clear sources
    Audio_clearAllSources();
    //memset(g_audio_event_bus.buffer, 0, g_audio_event_bus.capacity * g_audio_event_bus.element_size);
    LOGGER_INFO("Audio: Shutting down audio");
}

void Audio_play(AudioId id) {    
    Audio_raiseEvent(id, AUDIO_EVENT_PLAY);
}

void Audio_pause(AudioId id) {    
    Audio_raiseEvent(id, AUDIO_EVENT_PAUSE);    
}

void Audio_resume(AudioId id) {    
    Audio_raiseEvent(id, AUDIO_EVENT_RESUME);
}    

void Audio_stop(AudioId id) {    
    Audio_raiseEvent(id, AUDIO_EVENT_STOP);
}    

void Audio_processEvents() {
    AudioEvent audioEvent;

    // Use a 'while' loop, NOT an 'if'.
    // This ensures we process all pending events in a single frame if needed,
    // preventing a backlog.
    while (CB_read(&g_audio_event_bus, &audioEvent)) {
        Audio* audio = &g_audio_sources[ audioEvent.audioId ]; 

        switch (audioEvent.type) {
            
            case AUDIO_EVENT_PLAY:                
                if( audio->type ==  AUDIO_TYPE_SFX) {                    
                    SOUND_DRIVER_PCM_PLAY(0, audio->source, audio->size, SOUND_PCM_CH2, 7);
                    return;
                }
                SOUND_DRIVER_PLAY(audio->source);
                break;

            case AUDIO_EVENT_PAUSE:
                SOUND_DRIVER_PAUSE();
                break;

            case AUDIO_EVENT_RESUME:
                SOUND_DRIVER_RESUME();
                break;

            case AUDIO_EVENT_STOP:
                if( audio->type ==  AUDIO_TYPE_SFX) {
                    XGM2_stopPCM(SOUND_PCM_CH2);
                    return;
                }             
                SOUND_DRIVER_STOP();
                break;

            default:
                LOGGER_WARN("Audio: Failed to process event. Unknon type %d", audioEvent.type);
                break;
        }
        
    } 
}