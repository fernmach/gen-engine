/**
 *  \file config.h
 *  \brief Engine configuration file
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Defines some specific compilation option of the engine.
 */

#ifndef _ENG_CONFIG_H_
#define _ENG_CONFIG_H_

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
#define MEMORY_DEBUG_ENABLED 0

/**
 *  \brief
 *      Maximum number of listeners that can subscribe to a single event type.
 *      Adjust based on your game's needs and available RAM.
 */
#define EVENT_MAX_LISTENERS_PER_EVENT_TYPE 8

/**
 *  \brief
 *      Identify if the joypad 2, 3, 4... will be used in the game
 */
// #define 	JOY_1   0x0000 
// #define 	JOY_2   0x0001 
// #define 	JOY_3   0x0002 
// #define 	JOY_4   0x0003 
// #define 	JOY_5   0x0004 
// #define 	JOY_6   0x0005 
// #define 	JOY_7   0x0006 
// #define 	JOY_8   0x0007 
// #define 	JOY_NUM   0x0008 
// #define 	JOY_ALL   0xFFFF
#define INPUT_MAX_JOYPADS JOY_2

/**
 *  \brief
 *      Adjusts the maximum number of entities that can exists in memory.
 *      Adjust based on your game's needs and available RAM. 
 */
#define ECS_MAX_ENTITIES 64

/**
 *  \brief
 *      Adjusts the maximum number of components that can exists in memory.
 *      Adjust based on your game's needs and available RAM. 
 */
#define ECS_MAX_COMPONENTS 32

/**
 *  \brief
 *      Adjusts the size of the audio event buffer.
 *      Adjust based on your game's needs and available RAM.
 */
#define AUDIO_BUFFER_CAPACITY 16

/**
 *  \brief
 *      Adjusts the maximum number of audio sources that can exists in memory.
 *      Adjust based on your game's needs and available RAM.
 */
#define AUDIO_MAX_SOURCES ECS_MAX_ENTITIES

/**
 *  \brief
 *      Sets the XGM driver version: 1 = XGM, 2 = XGM2
 *      Adjust based on your game's needs and available RAM.
 */
#define AUDIO_XGM_DRIVER_VERSION 2

/**
 *  \brief
 *      Set to 1 to turn game engine profiler. 
 */
#define PROFILER_ENABLED 0

/**
 *  \brief
 *      Set to 1 to turn game engine CPU load and FPS display. 
 */
#define PROFILER_FPS_CPU_ENABLED 1

/**
 *  \brief
 *      Set to 1 to turn on the game engine Audio CPU load indicator. 
 */
#define PROFILER_AUDIO_CPU_LOAD_ENEBLED 1

/**
 *  \brief
 *      Set to 1 to display collider borders during gameplay
 */
#define DEBUG_COLLIDER_BORDERS_ENABLED 0

/**
 *  \brief
 *      Name of first scene to be loaded then the engine starts
 *      If not set the scene_main will be consiered the main one
 */
//#define STRINGIFY(value) #value
//#define ENGINE_MAIN_SCENE_VARIABLE STRINGIFY(main_scene)

// // FAT16 need EVERDRIVE
// #if ((MODULE_EVERDRIVE == 0) && (MODULE_FAT16 != 0))
// #error "Cannot enable FAT16 module without EVERDRIVE module"
// #endif


// char: 1 byte. This is guaranteed by the C standard.
// short int: Typically 2 bytes. The standard guarantees it is at least 2 bytes.
// int: Typically 4 bytes on most modern systems (32-bit or 64-bit). The standard guarantees it is at least as large as short int and at least 2 bytes.
// long int: Typically 4 or 8 bytes, depending on the system. The standard guarantees it is at least as large as int and at least 4 bytes.
// long long int: Typically 8 bytes. This type was introduced in C99 and is guaranteed to be at least 8 bytes.
// float: Typically 4 bytes.
// double: Typically 8 bytes.
// long double: Typically 8, 10, or 16 bytes, depending on the system and compiler.

#endif // _ENG_CONFIG_H_