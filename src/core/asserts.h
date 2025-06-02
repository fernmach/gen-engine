/**
 *  \file asserts.h
 *  \brief Engine asserts
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Provides wrappers for engine/game asserts (Uses Gens kDebug methods)
 */

#ifndef _ENG_ASSERTS_H_
#define _ENG_ASSERTS_H_

#include "config.h"

#if ASSERTS_ENABLED

    #include "logger.h"

    //SGDK KDebug forward declaration
    extern void KDebug_Halt(void);

    void Assert_reportFailure(const char *expression, const char *message, const char *file, int line);

    #define ASSERT_EXP(expr)                                       \
        if(expr) {                                                 \
        } else {                                                   \
            Assert_reportFailure(#expr, "", __FILE__, __LINE__);   \
            VDP_drawText("ASSERTION FAILED (KDebug halt)", 1, 24); \
            KDebug_Halt();                                         \
        }                                                          

    #define ASSERT_MSG(expr, message)                                  \
        if(expr) {                                                     \
        } else {                                                       \
            Assert_reportFailure(#expr, message, __FILE__, __LINE__);  \
            VDP_drawText("ASSERTION FAILED (KDebug halt)", 1, 24);     \
            KDebug_Halt();                                             \
        }

#else

    #define Assert_reportFailure (const char *expression, const char *message, const char *file, int line)

    #define ASSERT_EXP(expr)
    #define ASSERT_MSG(expr, message)

#endif // ASSERTS_ENABLED

#endif // _ENG_ASSERTS_H_