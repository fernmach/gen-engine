#ifndef _ENG_PROFILER_H_
#define _ENG_PROFILER_H_

#include "config.h"

#if PROFILER_ENABLED

    #include <genesis.h>

    #define MAX_PROFILER_ENTRIES 16 // Max distinct code blocks you can profile
    #define PROFILER_NAME_MAX_LEN 14 // Max length for profiler entry names (for display)
    #define PROFILER_AVERAGE_FRAMES 60 // Number of frames to average results over

    // Helper for converting subticks to microseconds for calculations/display
    // 1 sub-tick = 1/76800 s.
    // So, to get microseconds: (subticks * 1,000,000) / 76800
    // Simplify: (subticks * 1000000 / 100) / (76800 / 100) = (subticks * 10000) / 768
    // Further simplify by dividing by 64 (GCD of 10000 and 768 is tricky, let's use a known factor):
    // (subticks * 10000/64) / (768/64) = (subticks * 156.25) / 12 -- still involves float
    // Let's use (subticks * 125) / 96  (multiplying by 1M/76800 = 1M/ (768*100) = 10000/768 = 125/9.6 -- bad)
    // Exact: 1,000,000 / 76,800 = 10000 / 768 = 2500 / 192 = 625 / 48
    #define PROFILER_SUBTICKS_TO_US(subticks) (((u32)(subticks) * 625) / 48)
    // For display, we might need to scale to avoid overflow if u64 isn't fully supported by sprintf in SGDK's newlib nano.
    // If values get too large for sprintf's %lu, we might need to show Kilo-uS or ms.

    typedef struct {
        char name[PROFILER_NAME_MAX_LEN + 1];
        u32 startTimeSubTicks;

        u32 totalSubTicksThisFrame;
        u32 callCountThisFrame;
        u32 minSubTicksPerCallThisFrame;
        u32 maxSubTicksPerCallThisFrame;

        u32 accumulatedTotalSubTicks; // Use u64 for accumulator to prevent overflow over many frames
        u32 accumulatedCallCount;
        u32 overallMinSubTicks;
        u32 overallMaxSubTicks;

        u32 avgSubTicksPerCall; // Will be calculated as accumulatedTotalSubTicks / accumulatedCallCount
        u32 avgSubTicksPerFrame; // Will be calculated as accumulatedTotalSubTicks / PROFILER_FINE_AVERAGE_FRAMES

        u16 avgCallsPerFrame;
        bool active;
    } ProfilerEntry;

    void Profiler_Init(void);

    // Returns an ID for the entry, or -1 if full or name too long
    s16 Profiler_Register(const char* name);

    void Profiler_StartFrame(void); // Call once at the very beginning of your game loop
    void Profiler_EndFrame(void);   // Call once at the very end of your game loop (before VSync wait)

    void Profiler_Begin(s16 entryId);
    void Profiler_End(s16 entryId);

    void Profiler_DrawStats(u16 x, u16 y); // Draw stats on screen
    void Profiler_ResetAccumulatedStats(void); // Optional: to reset averages

    // Helper macros for convenience
    // Usage:
    // MY_FUNCTION_PROFILER_ID will be a static variable local to the scope where PROFILE_SCOPE is used.
    // PROFILE_SCOPE(MY_FUNCTION_PROFILER_ID, "MyFunc");
    //   ... code to profile ...
    // PROFILE_END_SCOPE(MY_FUNCTION_PROFILER_ID);

    #define PROFILE_SCOPE(id_var, name_str) \
        static s16 id_var = -2; /* -2 means uninitialized */ \
        if (id_var == -2) { id_var = Profiler_Register(name_str); } \
        if (id_var >= 0) Profiler_Begin(id_var)

    #define PROFILE_END_SCOPE(id_var) \
        if (id_var >= 0) Profiler_End(id_var)

#else // ENABLE_PROFILER not defined (stub out everything)

    #define Profiler_Init()
    #define Profiler_Register(name)
    #define Profiler_StartFrame()
    #define Profiler_EndFrame()
    #define Profiler_Begin(entryId)
    #define Profiler_End(entryId)
    #define Profiler_DrawStats(x, y)
    #define Profiler_ResetAccumulatedStats()

    //#define PROFILE_SCOPE(id_var, name_str) static s16 id_var = -1; /* Keep compiler happy */
    #define PROFILE_SCOPE(id_var, name_str)
    #define PROFILE_END_SCOPE(id_var)
    #define PROFILER_SUBTICKS_TO_US(subticks)

#endif //PROFILER_ENABLED

#endif //_ENG_PROFILER_H_