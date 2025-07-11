#include "audio_buffer.h"

void AudioBuffer_init(AudioBuffer* cb, AudioEvent* buffer, u16 capacity) {
    cb->buffer = buffer;
    cb->capacity = capacity;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool AudioBuffer_write(AudioBuffer* cb, AudioEvent event) {
    // Check if the buffer is full
    if (AudioBuffer_isFull(cb)) {
        // You could also decide to overwrite the oldest data,
        // but for audio events, it's usually better to drop the new one.
        return FALSE;
    }

    // Write data to the head position
    cb->buffer[cb->head] = event;

    // Move head to the next position, wrapping around if necessary
    cb->head = (cb->head + 1) % cb->capacity;
    
    // Increment the item count
    cb->count++;

    return TRUE;
}

bool AudioBuffer_read(AudioBuffer* cb, AudioEvent* event_out) {
    // Check if the buffer is empty
    if (AudioBuffer_isEmpty(cb)) {
        return FALSE;
    }

    // Read data from the tail position
    *event_out = cb->buffer[cb->tail];

    // Move tail to the next position, wrapping around if necessary
    cb->tail = (cb->tail + 1) % cb->capacity;

    // Decrement the item count
    cb->count--;

    return TRUE;
}