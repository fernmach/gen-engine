// // In memory_wrapper.h:
// #ifndef _MEMORY_WRAPPER_H_
// #define _MEMORY_WRAPPER_H_

// // SGDK includes
// #include <types.h>
// #include <sys.h>

// // Engine includes
// #include "memory_types.h"

// // USED definition (from SGDK)
// #ifndef USED
//     #define USED        1
// #endif

// // Redefine all original malloc calls to the wrapper one.
// #define MEM_alloc(size) __wrapper_MEM_alloc(size);

// // Memory allocation wrapper definition
// void* __wrapper_MEM_alloc(u16 size);

// void* __custom_MEM_alloc(u16 size);

// #endif // _MEMORY_WRAPPER_H_