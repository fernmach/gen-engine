#ifndef _ENG_PROFILER_H_
#define _ENG_PROFILER_H_

#include "config.h"

#if PROFILER_ENABLED

    #include <genesis.h>

    #define MAX_PROFILER_ENTRIES 16 // Max distinct code blocks you can profile
    #define PROFILER_NAME_MAX_LEN 14 // Max length for profiler entry names (for display)
    #define PROFILER_AVERAGE_FRAMES 60 // Number of frames to average results over

    typedef struct {
        char name[PROFILER_NAME_MAX_LEN + 1];

        u32 HBlankStartTime; // Temporary storage for current call

        // Stats for the current frame
        u32 totalHBlanksThisFrame;
        u32 callCountThisFrame;
        u32 minHBlanksPerCallThisFrame;
        u32 maxHBlanksPerCallThisFrame;

        // Accumulated stats for averaging
        u32 accumulatedTotalHBlanks;
        u32 accumulatedCallCount;
        // Note: Averaging min/max over frames is tricky. Simpler to show current frame's min/max
        // or overall min/max since reset. Let's stick to overall for simplicity.
        u32 overallMinHBlanks;
        u32 overallMaxHBlanks;

        // Calculated averaged stats
        u32 avgHBlanksPerCall;      // accumulatedTotalHBlanks / accumulatedCallCount
        u32 avgHBlanksPerFrame;     // accumulatedTotalHBlanks / PROFILER_AVERAGE_FRAMES
        u16 avgCallsPerFrame;       // accumulatedCallCount / PROFILER_AVERAGE_FRAMES

        bool active; // Is this entry in use?
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

#endif //PROFILER_ENABLED

#endif //_ENG_PROFILER_H_