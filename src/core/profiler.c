#include "config.h"
#include "logger.h"
#include "asserts.h"
#include "profiler.h"


#if PROFILER_ENABLED

ProfilerEntry profilerEntries[MAX_PROFILER_ENTRIES];
u16 profilerFrameCount = 0; // For averaging

void Profiler_Init(void) {
    memset(profilerEntries, 0, sizeof(profilerEntries));
    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        profilerEntries[i].active = FALSE;
        profilerEntries[i].overallMinSubTicks = MAX_U32; // Max u32
    }
    profilerFrameCount = 0;    
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
            profilerEntries[i].overallMinSubTicks = MAX_U32;
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
            profilerEntries[i].totalSubTicksThisFrame = 0;
            profilerEntries[i].callCountThisFrame = 0;
            profilerEntries[i].minSubTicksPerCallThisFrame = MAX_U32;
            profilerEntries[i].maxSubTicksPerCallThisFrame = 0;
        }
    }
}

void Profiler_Begin(s16 entryId) {
     if (entryId < 0 || entryId >= MAX_PROFILER_ENTRIES || !profilerEntries[entryId].active) return;
    // For getSubTick, we generally don't need to disable interrupts, as its VBlank handling
    // is internal. Disabling interrupts could slightly affect the measured code itself.
    profilerEntries[entryId].startTimeSubTicks = getSubTick();
}

void Profiler_End(s16 entryId) {
    if (entryId < 0 || entryId >= MAX_PROFILER_ENTRIES || !profilerEntries[entryId].active) return;

    u32 endTimeSubTicks = getSubTick();
    u32 duration_subticks;

    // getSubTick() is u32 and should wrap correctly for subtractions within its cycle.
    // A full cycle of u32 subticks at 13us/subtick is ~15.5 hours. Fine for profiling.
    duration_subticks = endTimeSubTicks - profilerEntries[entryId].startTimeSubTicks;

    ProfilerEntry* entry = &profilerEntries[entryId];
    entry->totalSubTicksThisFrame += duration_subticks;
    entry->callCountThisFrame++;

    if (duration_subticks < entry->minSubTicksPerCallThisFrame) {
        entry->minSubTicksPerCallThisFrame = duration_subticks;
    }
    if (duration_subticks > entry->maxSubTicksPerCallThisFrame) {
        entry->maxSubTicksPerCallThisFrame = duration_subticks;
    }
}

void Profiler_EndFrame(void) {
    profilerFrameCount++;

    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            ProfilerEntry* entry = &profilerEntries[i];
            entry->accumulatedTotalSubTicks += entry->totalSubTicksThisFrame;
            entry->accumulatedCallCount += entry->callCountThisFrame;

            if (entry->callCountThisFrame > 0) {
                if (entry->minSubTicksPerCallThisFrame < entry->overallMinSubTicks) {
                    entry->overallMinSubTicks = entry->minSubTicksPerCallThisFrame;
                }
                if (entry->maxSubTicksPerCallThisFrame > entry->overallMaxSubTicks) {
                    entry->overallMaxSubTicks = entry->maxSubTicksPerCallThisFrame;
                }
            }

            if (profilerFrameCount >= PROFILER_AVERAGE_FRAMES) {
                if (entry->accumulatedCallCount > 0) {
                    entry->avgSubTicksPerCall = (u32)(entry->accumulatedTotalSubTicks / entry->accumulatedCallCount);
                } else {
                    entry->avgSubTicksPerCall = 0;
                }
                entry->avgSubTicksPerFrame = (u32)(entry->accumulatedTotalSubTicks / PROFILER_AVERAGE_FRAMES);
                entry->avgCallsPerFrame = entry->accumulatedCallCount / PROFILER_AVERAGE_FRAMES; // This should be u16 or u32
            }
        }
    }

    if (profilerFrameCount >= PROFILER_AVERAGE_FRAMES) {
        profilerFrameCount = 0;
        for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
            if (profilerEntries[i].active) {
                 profilerEntries[i].accumulatedTotalSubTicks = 0;
                 profilerEntries[i].accumulatedCallCount = 0;
            }
        }
    }
}

void Profiler_ResetAccumulatedStats(void) {
    profilerFrameCount = 0;
    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            profilerEntries[i].accumulatedTotalSubTicks = 0;
            profilerEntries[i].accumulatedCallCount = 0;
            profilerEntries[i].avgSubTicksPerCall = 0;
            profilerEntries[i].avgSubTicksPerFrame = 0;
            profilerEntries[i].avgCallsPerFrame = 0;
            profilerEntries[i].overallMinSubTicks = MAX_U32;
            profilerEntries[i].overallMaxSubTicks = 0;
        }
    }
}

void Profiler_DrawStats(u16 x, u16 y) {
    char buffer[40]; // Max SGDK line width
    VDP_setTextPalette(0);

    sprintf(buffer, "Profiler(us) Avg/%df", PROFILER_AVERAGE_FRAMES);
    VDP_drawText(buffer, x, y++);
    //VDP_drawText("Name         TotFr Call MinC MaxC AvgC AvgFr", x, y++);
    // Col Widths:   12           6      3    4    4    4    5   = 38 + spaces
    VDP_drawText("Name         TotFr MinC MaxC AvgC AvgFr", x, y++);
    // Col Widths:   12           6      3    4    4    4    5   = 38 + spaces

    for (int i = 0; i < MAX_PROFILER_ENTRIES; ++i) {
        if (profilerEntries[i].active) {
            ProfilerEntry* entry = &profilerEntries[i];

            u32 us_total_frame = PROFILER_SUBTICKS_TO_US(entry->totalSubTicksThisFrame);
            u32 us_min_call_curr = (entry->minSubTicksPerCallThisFrame == MAX_U32) ? 0 : PROFILER_SUBTICKS_TO_US(entry->minSubTicksPerCallThisFrame);
            u32 us_max_call_curr = PROFILER_SUBTICKS_TO_US(entry->maxSubTicksPerCallThisFrame);
            u32 us_avg_call = PROFILER_SUBTICKS_TO_US(entry->avgSubTicksPerCall);
            u32 us_avg_frame = PROFILER_SUBTICKS_TO_US(entry->avgSubTicksPerFrame);


            // Make sure sprintf and buffer can handle the numbers.
            // %lu should be fine for u32. If us_total_frame > ~4.2M it overflows u32.
            // Max frame time in subticks: (1/60s) * 76800 subticks/s = 1280 subticks for NTSC.
            // Max us: 1280 * 625 / 48 = 16666 us. Fits u32.
            // sprintf(buffer, "%-*.*s %6lu %3lu %4lu %4lu %4lu %5lu",
            //         PROFILER_NAME_MAX_LEN, PROFILER_NAME_MAX_LEN, entry->name,
            //         us_total_frame,
            //         entry->callCountThisFrame,
            //         us_min_call_curr,
            //         us_max_call_curr,
            //         us_avg_call,
            //         us_avg_frame);
            sprintf(buffer, "%-*.*s %5lu %4lu %4lu %4lu %5lu",
                    PROFILER_NAME_MAX_LEN-3, PROFILER_NAME_MAX_LEN-3, entry->name,
                    us_total_frame,                    
                    us_min_call_curr,
                    us_max_call_curr,
                    us_avg_call,
                    us_avg_frame);
            VDP_drawText(buffer, x, y++);
            if (y >= (VDP_getScreenHeight() / 8) -1 ) break;
        }
    }
}

#endif