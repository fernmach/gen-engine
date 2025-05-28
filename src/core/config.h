/**
 *  \file config.h
 *  \brief Engine configuration file
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Defines some specific compilation option of the engine.
 */

#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

/**
 *  \brief
 *      Set to 1 to turn all game engine logging enabled.
 *      Relies on SGDK's Gens KMod logging module (LIB_DEBUG = 1)
 */
// #if (DEBUG && LIB_DEBUG)
//     #define LOGGER_ENABLED 1
// #else
//     #define LOGGER_ENABLED 1
// #endif
#define LOGGER_ENABLED 1

/**
 *  \brief
 *      Set to 1 to turn all game engine assertions enabled.
 *      Relies on SGDK's Gens KDebug module (LIB_DEBUG = 1)
 */
// #if (DEBUG && LIB_DEBUG)
//     #define ASSERTIONS_ENABLED 1
// #else
//     #define ASSERTIONS_ENABLED 0
// #endif
#define ASSERTS_ENABLED 1

/**
 *  \brief
 *      Set to 1 for memory debugging, 0 for production 
 */
// #if (DEBUG && LIB_DEBUG)
//     #define MEM_DEBUG_ENABLED 1
// #else
//     #define MEM_DEBUG_ENABLED 0
// #endif
#define MEMORY_DEBUG_ENABLED 1

/**
 *  \brief
 *      Maximum number of listeners that can subscribe to a single event type.
 *      Adjust based on your game's needs and available RAM.
 */
#define EVENT_MAX_LISTENERS_PER_EVENT_TYPE 8

/**
 *  \brief
 *      Identify if the joypad 2 will be used in the game
 */
#define INPUT_JOYPAD_2_ENABLED  1

// // FAT16 need EVERDRIVE
// #if ((MODULE_EVERDRIVE == 0) && (MODULE_FAT16 != 0))
// #error "Cannot enable FAT16 module without EVERDRIVE module"
// #endif


#endif // _ENGINE_CONFIG_H_

