/**
 *  \file memory.h
 *  \brief Engine memory allocation
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Defines memory allocation routines at engine levels for memory usage tracking.
 * Wraps all reoutines arround SGDK's default ones to tag memory usage
 */

#ifndef _ENG_MEMORY_H_
#define _ENG_MEMORY_H_

//#include <genesis.h>      // For sgdk/inc/memory.h types and functions
#include "config.h"         // Or ensure MEM_DEBUG_ENABLED is defined

// Attempt to create a memory wrapper to get insights from SGDKs inner functions
//#include "memory_types.h"
//#include "memory_wrapper.h"

// SGDK forward declarations
typedef unsigned long u32;
typedef unsigned short u16;

#if MEMORY_DEBUG_ENABLED

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

    // Initialize memory subsystem(tracking)
    void Memory_init();

    // Prototypes for your custom tracking functions
    void* Memory_alloc(u32 size, MemoryTag tag, const char* file, int line);
    void Memory_free(void* block, u32 size, MemoryTag tag, const char* file, int line);

    // Just add an allocation, or meme set call to the tracker
    void Memory_track(u32 size, MemoryTag tag);

    // Memory allocation report usage
    void Memory_reportUsage();

    //extern void* __wrapper_MEM_alloc(u16 size);

    // Wrapper Macros - these will pass file and line info automatically
    #define MEMORY_ALLOC(size, tag) Memory_alloc(size, tag, __FILE__, __LINE__)
    #define MEMORY_FREE(block, size, tag) Memory_free(block, size, tag, __FILE__, __LINE__)
    #define MEMORY_TRACK(size, tag) Memory_track(size, tag)

    // Optional: Override SGDK's macros/functions if you want a drop-in replacement
    // Be cautious with this, especially if SGDK internal files use them directly
    // without including your header. It's often safer to use your MY_MEM_* macros explicitly.    
    //#define MEM_alloc(size) Memory_alloc(size, MEM_TAG_UNKNOWN, __FILE__, __LINE__)    
#else

    // Use SGDK's functions directly
    #define MEMORY_ALLOC(size, tag) MEM_alloc(size)
    #define MEMORY_TRACK(size, tag)    
    
    // No-op for tracking functions in production
    #define Memory_init()
    #define Memory_alloc(size, tag, file, line)
    #define Memory_reportUsage()
    #define Memory_track(size, tag)


#endif // MEMORY_DEBUG_ENABLED

#endif // _ENG_MEMORY_H_