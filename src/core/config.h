/**
 *  \file config.h
 *  \brief Engine configuration file
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * This unit is used to define some specific compilation option of the engine.
 */

#ifndef _ENGINE_CONFIG_H_
#define _ENGINE_CONFIG_H_

/**
 *  \brief
 *      Set to 1 to turn all game engine logging enabled.
 *      Relies on KLog SGDK's KDebug logging (Gens KMod) module to operate properly (LIB_DEBUG = 1)
 */
// #if (DEBUG && LIB_DEBUG)    
//     #define LOGGER_ENABLED 1
// #else
//     #define LOGGER_ENABLED 1
// #endif
#define LOGGER_ENABLED 1

// // FAT16 need EVERDRIVE
// #if ((MODULE_EVERDRIVE == 0) && (MODULE_FAT16 != 0))
// #error "Cannot enable FAT16 module without EVERDRIVE module"
// #endif


#endif // _ENGINE_CONFIG_H_

