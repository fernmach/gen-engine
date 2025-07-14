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

#endif // _ENG_DEFNINES_H_