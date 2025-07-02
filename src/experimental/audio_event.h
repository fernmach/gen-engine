/*
// #ifndef _ENG_AUDIO_EVENTS_H_
// #define _ENG_AUDIO_EVENTS_H_

// #include "config.h"
// #include "logger.h"
// #include "asserts.h"
// #include "audio_defines.h"
// #include "containers/circular_buffer.h"

// #include <genesis.h>

// // Define the audio events your game will have.
// // Using an enum makes the code much more readable.
// typedef enum {
//     AUDIO_EVENT_NONE = 0, // Good practice to have a 'none' event
//     AUDIO_EVENT_PLAY,
//     SOUND_EVENT_PAUSE,
//     SOUND_EVENT_STOP,
//     SOUND_MAX_EVENT_TYPES
// } AudioEventType;

// typedef struct {
//     AudioId audioId;
//     AudioEventType type;
// } AudioEvent;

// // This is the control structure for our buffer.
// extern CircularBuffer audio_event_bus;

// // This is the actual memory for the buffer.
// // 'static' is important to ensure it's not on the stack.
// extern AudioEvent audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

// // Initialize the event bus.
// void AudioEvent_init();
// void AudioEvent_publish(AudioEvent event);
// void AudioEvent_processEvents();

// #endif // _ENG_AUDIO_EVENTS_H_
*/