#include "../logger.h"
#include "audio_buffer.h"
#include "audio.h"

// This is the control structure for our buffer.
AudioBuffer g_audio_event_bus;

// This is the actual memory for the audio event buffer.
AudioEvent g_audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

void Audio_init() {
    memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );

      // -- Initialize the Audio Event Bus --
    AudioBuffer_init(&g_audio_event_bus,
            g_audio_event_buffer_data, // Cast the struct array to a byte pointer
            AUDIO_BUFFER_CAPACITY); // Tell the buffer the size of our struct
    
    LOGGER_INFO("Audio subsystem initialized. Capacity %d", AUDIO_BUFFER_CAPACITY);
}

void Audio_clearBuffer() {

    // Check if we have a music playuing
    if ( AUDIO_MUSIC_DRIVER_IS_PLAYING() ) {
        AUDIO_MUSIC_DRIVER_STOP();
    }

    if ( AUDIO_PCM_DRIVER_IS_PLAYING(SOUND_PCM_CH1_MSK) == SOUND_PCM_CH1_MSK ) {
        AUDIO_PCM_DRIVER_STOP(SOUND_PCM_CH1);
    }

    for (u8 i = 1; i < g_audio_event_bus.count; ++i) {
        AudioEvent* event = &g_audio_event_buffer_data[i];
        event->data.source.sample = NULL;        
        event = NULL;
    }

    //Celar the audio buss
    AudioBuffer_clear(&g_audio_event_bus);

    //Clear the 
    memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );
    
    LOGGER_INFO("Audio: Clearing all buffer entries");
}

void Audio_shutdown() {
    //Clear sources
    Audio_clearBuffer();
    LOGGER_INFO("Audio: Shutting down audio");
}

