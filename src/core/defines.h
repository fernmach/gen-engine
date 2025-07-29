/**
 *  \file defines.h
 *  \brief Engine global definition file
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * Global engine definitions.
 */

#ifndef _ENG_DEFNINES_H_
#define _ENG_DEFNINES_H_

//Forward declare SGDK
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

/**
 *  \brief
 *      Forward declaration helper when using EntityId
 */
#if ECS_MAX_ENTITIES > 255
    typedef u16 EntityId;    
#else
    typedef u8 EntityId;
#endif

/**
 *  \brief
 *      If used, forces a function to be inlined.
 */
#ifndef INLINE
#define INLINE __attribute__((always_inline))
#endif

/**
 *  \brief
 *      Defides an F16 number by two (faster then using the F16_div)
 */
#define F16_half(val) ((val) >> 1)

/**
 *  \brief
 *      Defides a constant to indicate that a bool variable is not TRUE or FALSE, but undefined;
 */
#define UNDEFINED_BOOL 3

/**
 *  \brief
 *      Defides a constant to indicate that the a S16 value is undefined, such as a sprite depth property; 
 *      MAX_S16-1
 *      32767-1 = 32766
 */
#define UNDEFINED_S16 MAX_S16-1

/**
 *  \brief
 *      Defides a constant to indicate that the a S16 value is undefined, such as a sprite visibility property;
 *      MAX_U16-1
 *      32767-1 = 32766
 */
#define UNDEFINED_U16 MAX_U16-1

#endif // _ENG_DEFNINES_H_