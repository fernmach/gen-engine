/**
 *  \file logger.h
 *  \brief Engine logger/game logger
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * This unit provides a wrappers arround for engine/game level loggers
 * (Uses Gens KMod logging methods)
 */

#ifndef _ENG_LOGGER_H_
#define _ENG_LOGGER_H_

#include "config.h"

#if LOGGER_ENABLED

    // Forward declaration for SGDK types
    #include <genesis.h>

    typedef enum {    
        LOGGER_LEVEL_FATAL,     // Unrecoverable (crash)
        LOGGER_LEVEL_ERROR,     // Recoverable
        LOGGER_LEVEL_WARNING,   // Sub optimal operations
        LOGGER_LEVEL_INFO,      // Informational purpuses
        LOGGER_LEVEL_DEBUG,     // Debug level information (debug build only)
        LOGGER_LEVEL_TRACE,     // More verbose debug level (debug build only)
        LOGGER_LEVEL_DISABLED // To disable logging
    } LoggerLevel;

    // --- Configuration ---
    #define LOGGER_BUFFER_SIZE 256 // Max length of a single log message

    // --- Public API Functions ---
    // Initialize the logging system. Call once at the start.
    void Logger_init(LoggerLevel initial_level);

    // Core logging function (usually not called directly, use macros)
    void Logger_output(LoggerLevel level, const char* message, ...);

    // Enable/disable all logging at runtime
    void Logger_setEnabled(bool enabled);
    bool Logger_isEnabled(void);

    // Set the minimum log level to output
    void Logger_setLevel(LoggerLevel level);
    LoggerLevel Logger_getLevel(void);

    // Finalize the logging system. Call once at engine shutdown.
    void Logger_shutdown();

    // --- Public API Macros ---
    // These macros will pass __FILE__ and __LINE__ automatically
    #define LOGGER_FATAL(message, ...)   Logger_output(LOGGER_LEVEL_FATAL, message, ##__VA_ARGS__)
    #define LOGGER_ERROR(message, ...)   Logger_output(LOGGER_LEVEL_ERROR, message, ##__VA_ARGS__)
    #define LOGGER_WARN(message, ...)    Logger_output(LOGGER_LEVEL_WARNING, message, ##__VA_ARGS__)
    #define LOGGER_INFO(message, ...)    Logger_output(LOGGER_LEVEL_INFO, message, ##__VA_ARGS__)
    #define LOGGER_DEBUG(message, ...)   Logger_output(LOGGER_LEVEL_DEBUG, message, ##__VA_ARGS__)
    #define LOGGER_TRACE(message, ...)   Logger_output(LOGGER_LEVEL_TRACE, message, ##__VA_ARGS__)

#else

    // Define empty macros and stubs so the code compiles without logging
    #define Logger_init(initial_level)
    #define Logger_output(enabled)
    #define Logger_shutdown()
    #define Logger_setEnabled(enabled)
    #define Logger_isEnabled()
    #define Logger_setLevel(level)
    #define Logger_getLevel(void)

    #define LOGGER_DEBUG(fmt, ...)
    #define LOGGER_INFO(fmt, ...)
    #define LOGGER_WARN(fmt, ...)
    #define LOGGER_ERROR(fmt, ...)
    #define LOGGER_FATAL(fmt, ...)

#endif // LOGGER_ENABLED

#endif // _ENG_LOGGER_H_