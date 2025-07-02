/*
// #include "audio_event.h"

// // This is the actual memory for the buffer.
// // 'static' is important to ensure it's not on the stack.
// AudioEvent audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

// // This is the control structure for our buffer.
// CircularBuffer audio_event_bus;

// // Initialize the event bus.
// void AudioEvent_init() {
//     memset(audio_event_buffer_data, 0, sizeof(audio_event_buffer_data));

//       // -- Initialize the VFX Event Bus --
//     CB_init(&audio_event_bus, 
//             (u8*)audio_event_buffer_data, // Cast the struct array to a byte pointer
//             AUDIO_BUFFER_CAPACITY, 
//             sizeof(AudioEvent)); // Tell the buffer the size of our struct

//     //after initialization:
//     //LOGGER_DEBUG("CB after: Capacity %d, Count %d", audio_event_bus.capacity,  audio_event_bus.count);
// }

// void AudioEvent_publish(AudioEvent event) {
//     // Try to add a "shoot" event.
//     LOGGER_DEBUG("Audio: Size: %d, Cap: %d, Count: %d",
//         AUDIO_BUFFER_CAPACITY, audio_event_bus.capacity,
//         audio_event_bus.count
//     );

//     if ( !CB_write( &audio_event_bus, &event ) ) {
//         LOGGER_WARN("Audio Event: buffer full");
//         // Optional: Log an error if the buffer is full
//         // KLog("Audio buffer full! Dropped SHOOT event.");
//     }
// }

// void AudioEvent_processEvents() {
//      AudioEvent audioEvent;

//     // Use a 'while' loop, NOT an 'if'.
//     // This ensures we process all pending events in a single frame if needed,
//     // preventing a backlog.
//     while (CB_read(&audio_event_bus, &audioEvent)) {
        
//         // For debugging
//         // char dbg_str[32];
//         // sprintf(dbg_str, "Playing event: %d", event_to_play);
//         // VDP_clearText(10, 15, 20);
//         // VDP_drawText(dbg_str, 10, 15);        

//         switch (audioEvent.type) {
//             case AUDIO_EVENT_PLAY:
//                 // Replace 'sfx_jump' with your actual resource name from resources.res
//                 // Use SND_startPlay_PCM for PCM sounds or SND_startPlay_XGM for XGM sounds
//                 SOUND_DRIVER_PLAY(
//                     audio_event_buffer_data[ audioEvent.audioId ]
//                 );  
//                 ASSERT_MSG(1==0, "PLAYING SOUND");
//                 break;
//             default:
//                 // An unknown event was received. Ignore it.
//                 break;
//         }
        
//     }    
// }

// void Audio_play(AudioId id) {
//     // Create the audio event
//     AudioEvent event = {
//         .type = AUDIO_EVENT_PLAY,
//         .audioId = id
//     };

//     // Write it to the buffer
//     if ( !CB_write( &audio_event_bus, &event ) ) {
//         LOGGER_WARN("Audio Event: buffer full");
//         // Optional: Log an error if the buffer is full
//         // KLog("Audio buffer full! Dropped SHOOT event.");
//     }
// }
*/