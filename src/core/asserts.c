#include "config.h"

#if ASSERTS_ENABLED

#include "logger.h"

void Assert_reportFailure(const char* expression, const char* message, const char* file, int line) {        
    Logger_output(LOGGER_LEVEL_FATAL, "Assertion failure: %s, message: '%s', in file: %s, line: %d", expression, message, file, line);
}

#endif // ASSERTS_ENABLED