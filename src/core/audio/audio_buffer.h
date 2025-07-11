#pragma once
#ifndef _ENG_AUDIO_BUFFER_H_
#define _ENG_AUDIO_BUFFER_H_

#include "audio_types.h"

#include <genesis.h>

// The main circular buffer structure.
// This holds the state, but not the actual data array.
// This allows the data array to be allocated statically where you want.
typedef struct {
    //u8* buffer;     // Pointer to the data buffer
    AudioEvent* buffer;
    u16 head;       // Index to write to
    u16 tail;       // Index to read from
    u16 count;      // Number of items currently in the buffer
    u16 capacity;   // Total size of the buffer
} AudioBuffer;

/**
 * \brief Initializes a circular buffer structure.
 * \param cb        Pointer to the CircularBuffer structure to initialize.
 * \param buffer    Pointer to a pre-allocated byte array to be used for storage.
 * \param capacity  The total number of elements the buffer can hold.
 */
void AudioBuffer_init(AudioBuffer* cb, AudioEvent* buffer, u16 capacity);

/**
 * \brief Writes a new event to the circular buffer.
 * \param cb    Pointer to the circular buffer.
 * \param event The audio event (u8) to add.
 * \return      TRUE if the event was successfully added, FALSE if the buffer was full.
 */
bool AudioBuffer_write(AudioBuffer* cb, AudioEvent event);

/**
 * \brief Reads the oldest event from the circular buffer.
 * \param cb        Pointer to the circular buffer.
 * \param event_out Pointer to a u8 where the read event will be stored.
 * \return          TRUE if an event was successfully read, FALSE if the buffer was empty.
 */
bool AudioBuffer_read(AudioBuffer* cb, AudioEvent* event_out);

/**
 * \brief Clears the circular buffer, making it empty.
 * \param cb    Pointer to the circular buffer.
 * \note This is an O(1) operation. It does not erase the underlying data,
 *       it only resets the head, tail, and count.
 */
static inline void AudioBuffer_clear(AudioBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

/**
 * \brief Checks if the buffer is full.
 */
static inline bool AudioBuffer_isFull(const AudioBuffer* cb) {
    return cb->count == cb->capacity;
}

/**
 * \brief Checks if the buffer is empty.
 */
static inline bool AudioBuffer_isEmpty(const AudioBuffer* cb) {
    return cb->count == 0;
}

#endif // _ENG_AUDIO_BUFFER_H_