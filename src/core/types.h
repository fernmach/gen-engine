
/**
 *  \file types.h
 *  \brief Engine type definition
 *  \author Fernando Machado
 *  \date 31/2025
 *
 * Provides types definition to be used across all engine. 
 */
#ifndef _ENGINE_TYPES_H_
#define _ENGINE_TYPES_H_

// Forward declaration (SGDK)
// typedef short s16;
// typedef s16 fix16;
#include <genesis.h>

// --- Event Handler Function Pointer ---
typedef void (*PFN_onInit)(void);
typedef void (*PFN_onUpdate)(fix16 deltaTime);
typedef void (*PFN_onDraw)(void);
typedef void (*PFN_onDestroy)();
typedef void (*PFN_onShutdown)();


#endif //_ENGINE_TYPES_H_