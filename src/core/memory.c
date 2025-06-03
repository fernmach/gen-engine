#include "config.h"
#include "logger.h"
#include "asserts.h"
#include "memory.h"

#if MEMORY_DEBUG_ENABLED

#ifdef MEM_alloc
    #undef MEM_alloc
#endif

// --- Data Structures for Tracking ---
// #define MEMORY_ALLOCATIONS_GB 1024 * 1024 * 1024
// #define MEMORY_ALLOCATIONS_MB 1024 * 1024
// #define MEMORY_ALLOCATIONS_KB 1024

static u32 g_allocation_table[MEM_TAG_COUNT];
static u32 g_total_allocated_bytes;

// --- Helper Functions ---
const char* memory_tag_strings[MEM_TAG_COUNT] = {
    "Unknown", "Logger", "System", "Game State", "Level Data", "Sprite GFX",
    "Sprite PAL", "Sound Samples", "Music Track", "UI"
    // Add corresponding strings for your tags
};

// --- Public Tracking API Implementation ---

void Memory_init(void) {    
    g_total_allocated_bytes = 0;    
    memset(g_allocation_table, 0, sizeof(g_allocation_table));
    
    LOGGER_INFO("Memory tracker subsystem initialized.");
}

// Prototypes for your custom tracking functions
void* Memory_alloc(u32 size, MemoryTag tag, const char* file, int line) {
    g_total_allocated_bytes += size;
    g_allocation_table[tag] += size;

    void* block = MEM_alloc(size);

    // TODO: Add memory allocation verification
    //ASSERT_MSG(block, "MEM_ALLOC: Failed to allocate %d, bytes @ %s: %d", size, file, line);
    ASSERT_MSG(block, "Memory allocation failled");   
    
    return block;
}

void Memory_free(void* block, u32 size, MemoryTag tag, const char* file, int line) {
    if (!block) {
        LOGGER_WARN("Attempt to free untracked or already free pointer 0x%u from file %s, line: %d", (u32)block, file, line);
        return;
    }

    g_total_allocated_bytes -= size;
    g_allocation_table[tag] -= size;

    MEM_free(block);
}

void Memory_track(u32 size, MemoryTag tag) {
    g_total_allocated_bytes += size;
    g_allocation_table[tag] += size;
}

void Memory_reportUsage() {
    //TODO: Format byts in mega, giga and kilo  
    KLog("---------------------------");
    KLog("Memory Usage Report");
    
    //LOGGER_INFO ("Current total allocated: %lu bytes", g_total_allocated_bytes);
    //LOGGER_INFO ("    Current total allocated: %lu bytes", MEM_getAllocated());    
    //LOGGER_INFO ("    Current free memory: %lu bytes", MEM_getFree());
    
    kprintf("    Current total allocated (SGDK): %d bytes", MEM_getAllocated());
    kprintf("    Current free memory (SGDK): %d bytes", MEM_getFree());
    kprintf("    Engine total allocated: %lu bytes", g_total_allocated_bytes);
    
    KLog("Usage by tags:");
    for (MemoryTag tag = 0; tag < MEM_TAG_COUNT; ++tag) {        
        //if (g_allocation_table[tag] > 0) {
        kprintf("    %s: %lu bytes", memory_tag_strings[tag], g_allocation_table[tag]);
        //}
    }

    MEM_dump();

    KLog("---------------------------");
}

#endif // MEMORY_DEBUG_ENABLED