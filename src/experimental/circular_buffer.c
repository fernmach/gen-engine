#include "circular_buffer.h"
void CB_init(CircularBuffer* cb, u8* buffer_data, u16 capacity, u16 element_size) {
    cb->buffer = buffer_data;
    cb->capacity = capacity;
    cb->element_size = element_size;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool CB_write(CircularBuffer* cb, const void* element_data) {
    if (CB_is_full(cb)) {
        return FALSE;
    }

    // Calculate the memory address for the new element
    u8* dest = cb->buffer + (cb->head * cb->element_size);
    
    // Copy the element data into the buffer
    memcpy(dest, element_data, cb->element_size);

    // Advance the head
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count++;

    return TRUE;
}

bool CB_read(CircularBuffer* cb, void* element_out) {
    if (CB_is_empty(cb)) {
        return FALSE;
    }

    // Calculate the memory address of the element to read
    u8* src = cb->buffer + (cb->tail * cb->element_size);

    // Copy the element data from the buffer to the output variable
    memcpy(element_out, src, cb->element_size);

    // Advance the tail
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count--;

    return TRUE;
}