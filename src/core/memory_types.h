// In memory_wrapper.h:
#ifndef _MEMORY_TYPES_H
#define _MEMORY_TYPES_H

// Define memory tags/categories
typedef enum {
    MEM_TAG_UNKNOWN = 0,
    MEM_TAG_LOGGER,
    MEM_TAG_SYSTEM,
    MEM_TAG_GAME_STATE,
    MEM_TAG_LEVEL_DATA,
    MEM_TAG_SPRITE_GFX,
    MEM_TAG_SPRITE_PAL,
    MEM_TAG_SOUND_SAMPLES,
    MEM_TAG_MUSIC_TRACK,
    MEM_TAG_UI,
    // Add more tags as needed
    MEM_TAG_COUNT
} MemoryTag;

#endif // _MEMORY_TYPES_H