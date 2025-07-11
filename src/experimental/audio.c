/*
// #include "logger.h"
// #include "audio_types.h"
// #include "audio_defines.h"
// #include "./containers/audio_buffer.h"
// #include "audio.h"

// This is the control structure for our buffer.
static AudioBuffer g_audio_event_bus;

// This is the actual memory for the audio event buffer.
static AudioEvent g_audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

void Audio_init() {
    memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );

      // -- Initialize the Audio Event Bus --
    AB_init(&g_audio_event_bus,
            g_audio_event_buffer_data, // Cast the struct array to a byte pointer
            AUDIO_BUFFER_CAPACITY); // Tell the buffer the size of our struct
    
    LOGGER_INFO("Audio subsystem initialized. Capacity %d", AUDIO_BUFFER_CAPACITY);
}

void Audio_clearBuffer() {
    // // TODO: How to stop individual audio sources
    // for (u8 i = 0; i < g_audio_event_bus->count; ++i) {
    //     AudioEvent* event = &g_audio_event_buffer_data[i];

    //     if ( AUDIO_PCM_DRIVER_IS_PLAYING ( event->audio->sample ) ) {
    //         AUDIO_PCM_DRIVER_STOP ( event->audio->sample );
    //     }

    //     event->audio = NULL;
    //     event = NULL;
    // }

    // //Celar the audio buss
    // AB_clear(&g_audio_event_bus);

    // //Clear the 
    // memset( g_audio_event_buffer_data, 0, sizeof(g_audio_event_buffer_data) );
    
    LOGGER_INFO("Audio: Clearing all buffer entries");
}

void Audio_shutdown() {
    //Clear sources
    //Audio_clearBuffer();    
    //memset(g_audio_event_bus.buffer, 0, g_audio_event_bus.capacity * g_audio_event_bus.element_size);
    LOGGER_INFO("Audio: Shutting down audio");
}
    */