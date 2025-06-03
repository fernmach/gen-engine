#include "config.h"
#include "logger.h"
#include "asserts.h"
#include "profiler.h"


#ifdef PROFILER_ENABLED

ProfilerEntry profilerEntries[MAX_PROFILER_ENTRIES];
u16 profilerFrameCount = 0; // For averaging
u16 totalHBlanksInFrame = 0; // NTSC: 262, PAL: 313. Set in init.

void Profiler_Init(void) {
    memset(profilerEntries, 0, sizeof(profilerEntries));
    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        profilerEntries[i].active = FALSE;
        profilerEntries[i].overallMinHBlanks = 0xFFFFFFFF; // Max u32 value
    }
    profilerFrameCount = 0;

    // Determine total HBlanks for percentage calculations
    if (VDP_getScreenHeight() == 224) { // NTSC typically
        totalHBlanksInFrame = 262;
    } else { // PAL typically
        totalHBlanksInFrame = 313;
    }
}

s16 Profiler_Register(const char* name) {
    // if (strlen(name) > PROFILER_NAME_MAX_LEN) {
    //     // Optional: Log an error if your logger is available
    //     // Logger_Error("Profiler_Register: Name too long: %s", name);
    //     return -1; // Name too long
    // }
    ASSERT_MSG(strlen(name) < PROFILER_NAME_MAX_LEN, 
        "Profiler: Name exceded maximum allowed chars");

    for (s16 i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (!profilerEntries[i].active) {
            strncpy(profilerEntries[i].name, name, PROFILER_NAME_MAX_LEN);
            profilerEntries[i].name[PROFILER_NAME_MAX_LEN] = '\0'; // Ensure null termination
            profilerEntries[i].active = TRUE;
            profilerEntries[i].overallMinHBlanks = 0xFFFFFFFF;
            LOGGER_DEBUG("Profiler: Registered '%s' to ID %d", name, i);
            return i;
        }
    }
    LOGGER_WARN("Profiler_Register: No free entries for %s", name);
    return -1; // No free entries
}

void Profiler_StartFrame(void) {
    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            profilerEntries[i].totalHBlanksThisFrame = 0;
            profilerEntries[i].callCountThisFrame = 0;
            profilerEntries[i].minHBlanksPerCallThisFrame = 0xFFFFFFFF;
            profilerEntries[i].maxHBlanksPerCallThisFrame = 0;
        }
    }
}

void Profiler_Begin(s16 entryId) {
    if (entryId < 0 || entryId >= MAX_PROFILER_ENTRIES || !profilerEntries[entryId].active) return;
    // Critical: Disable interrupts to get a more stable HBlank reading if needed,
    // but VDP_getHBlankCounter is usually safe as it reads VDP state.
    // u16 oldInt = SYS_getInterruptMaskLevel();
    // SYS_setInterruptMaskLevel(0); // Or a higher level that masks VBlank
    profilerEntries[entryId].HBlankStartTime = VDP_getHIntCounter();
    // SYS_setInterruptMaskLevel(oldInt);
}

void Profiler_End(s16 entryId) {
    if (entryId < 0 || entryId >= MAX_PROFILER_ENTRIES || !profilerEntries[entryId].active) return;

    // u16 oldInt = SYS_getInterruptMaskLevel();
    // SYS_setInterruptMaskLevel(0);
    u32 endTime = VDP_getHIntCounter();
    // SYS_setInterruptMaskLevel(oldInt);

    u32 duration;
    // Handle HBlank counter wrapping around (though unlikely for typical function duration)
    if (endTime >= profilerEntries[entryId].HBlankStartTime) {
        duration = endTime - profilerEntries[entryId].HBlankStartTime;
    } else {
        // Wrapped around (e.g. start was 260, end was 5 on NTSC)
        duration = (totalHBlanksInFrame - profilerEntries[entryId].HBlankStartTime) + endTime;
    }

    ProfilerEntry* entry = &profilerEntries[entryId];
    entry->totalHBlanksThisFrame += duration;
    entry->callCountThisFrame++;

    if (duration < entry->minHBlanksPerCallThisFrame) {
        entry->minHBlanksPerCallThisFrame = duration;
    }
    if (duration > entry->maxHBlanksPerCallThisFrame) {
        entry->maxHBlanksPerCallThisFrame = duration;
    }
}

void Profiler_EndFrame(void) {
    profilerFrameCount++;

    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            ProfilerEntry* entry = &profilerEntries[i];
            entry->accumulatedTotalHBlanks += entry->totalHBlanksThisFrame;
            entry->accumulatedCallCount += entry->callCountThisFrame;

            if (entry->callCountThisFrame > 0) { // Avoid division by zero if not called
                if (entry->minHBlanksPerCallThisFrame < entry->overallMinHBlanks) {
                    entry->overallMinHBlanks = entry->minHBlanksPerCallThisFrame;
                }
                if (entry->maxHBlanksPerCallThisFrame > entry->overallMaxHBlanks) {
                    entry->overallMaxHBlanks = entry->maxHBlanksPerCallThisFrame;
                }
            }


            if (profilerFrameCount >= PROFILER_AVERAGE_FRAMES) {
                if (entry->accumulatedCallCount > 0) {
                    entry->avgHBlanksPerCall = entry->accumulatedTotalHBlanks / entry->accumulatedCallCount;
                } else {
                    entry->avgHBlanksPerCall = 0;
                }
                entry->avgHBlanksPerFrame = entry->accumulatedTotalHBlanks / PROFILER_AVERAGE_FRAMES;
                entry->avgCallsPerFrame = entry->accumulatedCallCount / PROFILER_AVERAGE_FRAMES;
            }
        }
    }

    if (profilerFrameCount >= PROFILER_AVERAGE_FRAMES) {
        profilerFrameCount = 0; // Reset for next averaging period
        // Also reset accumulators for entries that were active in this period
        for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
            if (profilerEntries[i].active) {
                profilerEntries[i].accumulatedTotalHBlanks = 0;
                profilerEntries[i].accumulatedCallCount = 0;
            }
        }
    }
}

void Profiler_ResetAccumulatedStats(void) {
    profilerFrameCount = 0;
    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            profilerEntries[i].accumulatedTotalHBlanks = 0;
            profilerEntries[i].accumulatedCallCount = 0;
            profilerEntries[i].avgHBlanksPerCall = 0;
            profilerEntries[i].avgHBlanksPerFrame = 0;
            profilerEntries[i].avgCallsPerFrame = 0;
            profilerEntries[i].overallMinHBlanks = 0xFFFFFFFF;
            profilerEntries[i].overallMaxHBlanks = 0;
        }
    }
}

void Profiler_DrawStats(u16 x, u16 y) {
    char buffer[40]; // Max line length for SGDK text is 40 chars
    VDP_setTextPalette(0); // Or your desired palette for debug text

    // Header
    sprintf(buffer, "Profiler (HBlanks) Avg/%df", PROFILER_AVERAGE_FRAMES);
    VDP_drawText(buffer, x, y++);
    VDP_drawText("Name         TotFr Cur C Min C Max C Avg C AvgFr", x, y++);
    //             UpdateLogic  12345 00123 00123 00123 00123 00123
    //             xxxxxxxxxxxx xxxxx xxxxx xxxxx xxxxx xxxxx xxxxx

    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            ProfilerEntry* entry = &profilerEntries[i];
            u32 currentFrameMin = entry->minHBlanksPerCallThisFrame == 0xFFFFFFFF ? 0 : entry->minHBlanksPerCallThisFrame;

            // Adjust spacing based on PROFILER_NAME_MAX_LEN
            // This formatting is a bit fiddly.
            sprintf(buffer, "%-*.*s %5lu %5lu %5lu %5lu %5lu %5lu",
                    PROFILER_NAME_MAX_LEN, PROFILER_NAME_MAX_LEN, entry->name,
                    entry->totalHBlanksThisFrame,
                    entry->callCountThisFrame,
                    currentFrameMin,
                    entry->maxHBlanksPerCallThisFrame,
                    entry->avgHBlanksPerCall,
                    entry->avgHBlanksPerFrame);
            VDP_drawText(buffer, x, y++);
            if (y >= (VDP_getScreenHeight() / 8) -1 ) break; // Don't draw off screen
        }
    }
}

#endif