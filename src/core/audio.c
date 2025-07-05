#include "logger.h"
#include "audio.h"

#include "./containers/circular_buffer.h"

// This is the control structure for our buffer.
static CircularBuffer g_audio_event_bus;

// This is the actual memory for the audio event buffer.
static AudioEvent g_audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

// List of audio sources
static Audio g_audio_sources[AUDIO_MAX_SOURCES];

// Count of how many audio sources are active(head of the array)
static AudioId g_audio_sources_count;

static AudioId current_music;
static AudioId next_music_request;
static AudioEventType current_music_next_event;

void Audio_init(const MusicDriver *music_driver, const SFXDriver sfx_driver) {
    // Zero out all component data and entity states
    memset( g_audio_sources, 0, sizeof(g_audio_sources) );
    memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );   

    g_audio_sources_count = 0;

      // -- Initialize the VFX Event Bus --
    CB_init(&g_audio_event_bus, 
            (u8*)g_audio_event_buffer_data, // Cast the struct array to a byte pointer
            AUDIO_BUFFER_CAPACITY, 
            sizeof(AudioEvent)); // Tell the buffer the size of our struct

    current_music = NULL;
    next_music_request = NULL;
    current_music_next_event = AUDIO_EVENT_NONE;

    // Identify if audio should be initialized here    
    Music_init(music_driver);
    LOGGER_INFO("Music subsystem initialized");

    SFX_init(sfx_driver);
    LOGGER_INFO("Sound effects subsystem initialized");

    LOGGER_INFO("Audio subsystem initialized. Buffer capacity %d", g_audio_event_bus.capacity);
}

AudioId Audio_addSource(Audio audio) {    
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
    // Stop playing the audio if necessary  
    if( g_audio_sources[ audioId ].type == AUDIO_TYPE_SFX ) {
        if ( SFX_isPlaying(g_audio_sources[ audioId ].channel) ) {
            SFX_stop( g_audio_sources[ audioId ].channel );
        }
    } else {
        if (Music_isPlaying()) {
            Music_stop();
        }
    }

    // Set the source as null
    g_audio_sources[ audioId ].source = NULL;

    // Decrement the active entity count. "pop" the source
    g_audio_sources_count--;

    // If the entity we are destroying is NOT the last one in the array,
    // we need to move the last one into its place to fill the gap.
    // Last Id  = g_audio_sources_count
    if (audioId != g_audio_sources_count) {
        // If the last posotion of the source is the current music
        // we need to shift it from place
        if (current_music == g_audio_sources_count) {
            current_music = audioId;
        }

        g_audio_sources[ audioId ] = g_audio_sources[ g_audio_sources_count ];
    }

    LOGGER_DEBUG("Audio: Removed source %d ", audioId);
}

void Audio_clearAllSources() {    
    for (AudioId i = 0; i < g_audio_sources_count; ++i) {
        Audio* audio = &g_audio_sources[ i ];

        if( audio->type == AUDIO_TYPE_SFX ) {
            if ( SFX_isPlaying(audio->channel) ) {
                SFX_stop( audio->channel );
            }
        } else {
            if (Music_isPlaying()) {
                Music_stop();
            }
        }

        audio->source = NULL;
    }

    //TODO: memset is slow evaluate necessit of doing it in real time
    //memset( g_audio_sources, 0, sizeof(g_audio_sources) );
    g_audio_sources_count = 0;

    //Celar the audio buss
    CB_clear(&g_audio_event_bus);

    LOGGER_INFO("Audio: Clearing all sources/buffer");
}

void Audio_shutdown() {
    Music_destroy();
    LOGGER_INFO("Audio: Shutting down music subsystem");

    SFX_destroy();
    LOGGER_INFO("Audio: Shutting down sfx subsystem");

    //Clear sources
    Audio_clearAllSources();

    //memset(g_audio_event_bus.buffer, 0, g_audio_event_bus.capacity * g_audio_event_bus.element_size);
    LOGGER_INFO("Audio: Shutting down audio");
}

void Audio_play(AudioId id) {
    if ( g_audio_sources[id].type == AUDIO_TYPE_SFX ) {
        Audio_raiseEvent(id, AUDIO_EVENT_PLAY);
    } else {
        current_music_next_event = AUDIO_EVENT_PLAY;
        next_music_request = id;
    }
}

void Audio_pause(AudioId id) {
    current_music_next_event = AUDIO_EVENT_PAUSE;        
}

void Audio_resume(AudioId id) {    
    current_music_next_event = AUDIO_EVENT_RESUME;
}    

void Audio_stop(AudioId id) {
    if ( g_audio_sources[id].type == AUDIO_TYPE_SFX ) {
        Audio_raiseEvent(id, AUDIO_EVENT_STOP);
    } else {
        current_music_next_event = AUDIO_EVENT_STOP;
    }
}

bool Audio_isPlaying() {
    if ( g_audio_sources[id].type == AUDIO_TYPE_SFX ) {        
        return SFX_isPlaying();
    } else {
        return Music_isPlaying();
    }
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
                    XGM_PCM_DRIVER_PLAY(0, audio->source, audio->size, SOUND_PCM_CH2, 7);
                    return;
                }
                XGM_MUSIC_DRIVER_PLAY(audio->source);
                break;

            case AUDIO_EVENT_PAUSE:
                XGM_MUSIC_DRIVER_PAUSE();
                break;

            case AUDIO_EVENT_RESUME:
                XGM_MUSIC_DRIVER_RESUME();
                break;

            case AUDIO_EVENT_STOP:
                if( audio->type ==  AUDIO_TYPE_SFX) {
                    XGM2_stopPCM(SOUND_PCM_CH2);
                    return;
                }             
                XGM_MUSIC_DRIVER_STOP();
                break;

            default:
                LOGGER_WARN("Audio: Failed to process event. Unknon type %d", audioEvent.type);
                break;
        }
        
    } 
}