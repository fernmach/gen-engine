#ifndef _ENG_CIRCULAR_BUFFER_H_
#define _ENG_CIRCULAR_BUFFER_H_

#include <genesis.h>


// The generic circular buffer structure.
typedef struct {
    u8* buffer;         // Pointer to the raw byte buffer for storage
    u16 head;           // Index to write the next element
    u16 tail;           // Index to read the oldest element
    u16 count;          // Number of items currently in the buffer
    u16 capacity;       // Total number of elements the buffer can hold
    u16 element_size;   // The size of a single element in bytes
} CircularBuffer;

/**
 * \brief Initializes a generic circular buffer.
 * \param cb            Pointer to the CircularBuffer structure to initialize.
 * \param buffer_data   Pointer to a pre-allocated byte array to be used for storage.
 *                      The total size of this array must be at least (capacity * element_size).
 * \param capacity      The total number of elements the buffer can hold.
 * \param element_size  The size of each element in bytes (e.g., sizeof(MyStruct)).
 */
void CB_init(CircularBuffer* cb, u8* buffer_data, u16 capacity, u16 element_size);

/**
 * \brief Writes a new element to the circular buffer by copying its data.
 * \param cb            Pointer to the circular buffer.
 * \param element_data  A pointer to the data to be copied into the buffer.
 * \return              TRUE if the element was successfully written, FALSE if the buffer was full.
 */
bool CB_write(CircularBuffer* cb, const void* element_data);

/**
 * \brief Reads the oldest element from the circular buffer, copying it to a destination.
 * \param cb            Pointer to the circular buffer.
 * \param element_out   A pointer to a variable where the read element will be copied.
 * \return              TRUE if an element was successfully read, FALSE if the buffer was empty.
 */
bool CB_read(CircularBuffer* cb, void* element_out);

/**
 * \brief Clears the circular buffer, making it empty.
 * \param cb    Pointer to the circular buffer.
 * \note This is an O(1) operation. It does not erase the underlying data,
 *       it only resets the head, tail, and count.
 */
static inline void CB_clear(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

/**
 * \brief Checks if the buffer is full.
 */
static inline bool CB_is_full(const CircularBuffer* cb) {
    return cb->count == cb->capacity;
}

/**
 * \brief Checks if the buffer is empty.
 */
static inline bool CB_is_empty(const CircularBuffer* cb) {
    return cb->count == 0;
}

#endif // _ENG_CIRCULAR_BUFFER_H_