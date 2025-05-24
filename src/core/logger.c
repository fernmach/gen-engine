#include "config.h"
#include "logger.h"

#if LOGGER_ENABLED

// --- Internal State ---
static bool             g_logger_enabled = TRUE;
static LoggerLevel      g_logger_current_level = LOGGER_LEVEL_DEBUG;
static char             g_logger_buffer[LOGGER_BUFFER_SIZE];

// --- Helper: Get Log Level Prefix ---
static const char* level_strings[] = {
  "FTL", "ERR", "WRN", "INF", "DBG", "TRC"
}; 

// --- Public API Functions ---
// Initialize the logging system. Call once at the start.
void Logger_init(LoggerLevel initial_level) {
    g_logger_current_level = initial_level;
    g_logger_enabled = TRUE;    
    
    // KLog is often initialized by SGDK itself, but good to be aware    
    LOGGER_INFO("Logger subsystem initialized. (Level: %s)", level_strings[initial_level]);
}

// Core logging function (usually not called directly, use macros)
void Logger_output(LoggerLevel level, const char *file, int line, const char *fmt, ...) {
    if (!g_logger_enabled || level < g_logger_current_level) {
        return;
    }

    char message_buffer[LOGGER_BUFFER_SIZE];
    va_list args;

    // 1. Format the user's message
    va_start(args, fmt);    
    vsprintf(message_buffer, fmt, args);
    va_end(args);

    // 2. Format the full log line (Prefix, File:Line, Message)
    // Extracting basename of file
    const char *basename = file;
    const char *p = file;
    while (*p) {
        if (*p == '/' || *p == '\\') {
            basename = p + 1;
        }
        p++;
    }
    
    // Ensure KLog is running (it usually is by default in debug builds of SGDK)
    // Output to KLog/KDebug (emulator console)    
    sprintf(g_logger_buffer, "[%s] %s:%d: %s",
            level_strings[level], basename, line, message_buffer);

    KLog(g_logger_buffer);
}

// Finalize the logging system. Call once at engine shutdown.
void Logger_shutdown() {
    // TODO: Cleanup loggin/write queued entries    

    LOGGER_INFO("Shutting down logger subsystem");
}

// Enable/disable all logging at runtime
void Logger_setEnabled(bool enabled) {
    g_logger_enabled = enabled;
}

bool Logger_isEnabled(void) {
    return g_logger_enabled;
}

// Set the minimum log level to output
void Logger_setLevel(LoggerLevel level) {
     g_logger_current_level = level;
}

LoggerLevel Logger_getLevel(void) {
    return g_logger_current_level;
}

#endif // LOGGER_ENABLED