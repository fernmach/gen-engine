// #include "memory_wrapper.h"

// #include <tab_cnv.h>
// #include <sys.h>
// #include <string.h>
// #include <kdebug.h>
// #include <tools.h>

// // SGDK forward
// static u16* free;
// static u16* heap;
// static u16* pack(u16 nsize);

// // Engine forward
// void Memory_track(u32 size, MemoryTag tag);

// // wrapper allocation
// NO_INLINE void* __wrapper_MEM_alloc(u16 size) {    
//     Memory_track(size, MEM_TAG_UNKNOWN);
//     void* block = __custom_MEM_alloc(size);
//     KLog("allocation memory");
//     return block;
// }

// NO_INLINE void* __custom_MEM_alloc(u16 size)
// {
//     u16* p;
//     u16 adjsize;
//     u16 remaining;

//     if (size == 0)
//         return 0;

//     // 2 bytes aligned
//     adjsize = (size + sizeof(u16) + 1) & 0xFFFE;

//     if (adjsize > *free)
//     {
//         p = pack(adjsize);

//         // no enough memory
//         if (p == NULL)
//         {
// #if (LIB_LOG_LEVEL >= LOG_LEVEL_ERROR)
//             if (size > MEM_getFree())
//                 KLog_U2_("MEM_alloc(", size, ") failed: no enough free memory (free = ", MEM_getFree(), ")");
//             else
//                 KLog_U3_("MEM_alloc(", size, ") failed: cannot find a big enough memory block (largest free block = ", MEM_getLargestFreeBlock(), " - free = ", MEM_getFree(), ")");
// #endif

//             return NULL;
//         }

//         free = p;
//     }
//     else
//         // at this point we can allocate memory
//         p = free;

//     // set free to next free block
//     free += adjsize >> 1;

//     // get remaining (old - allocated)
//     remaining = *p - adjsize;
//     // adjust remaining free space
//     if (remaining > 0) *free = remaining;
//     else
//     {
//         // no more space in block so we have to find the next free bloc
//         u16 *newfree = free;
//         u16 bloc;

//         while((bloc = *newfree) & USED)
//             newfree += bloc >> 1;

//         free = newfree;
//     }

//     // set block size, mark as used and point to free region
//     *p++ = adjsize | USED;

// #if (LIB_LOG_LEVEL >= LOG_LEVEL_INFO)
//     kprintf("MEM_alloc(%d) success: %lx - remaining = %d", size, (u32) p, MEM_getFree());
// #endif

//     // return block
//     return p;
// }


// /*
//  * Pack free block and return first matching free block.
//  */
// static u16* pack(u16 nsize)
// {
//     u16 *b;
//     u16 *best;
//     u16 bsize, psize;

//     b = heap;
//     best = b;
//     bsize = 0;

//     while ((psize = *b))
//     {
//         if (psize & USED)
//         {
//             if (bsize != 0)
//             {
//                 // store packed free size
//                 *best = bsize;

//                 // return it if greater than what we're looking for
//                 if (bsize >= nsize)
//                     return best;

//                 // reset packed free size
//                 bsize = 0;
//             }

//             // point to next memory block
//             b += psize >> 1;
//             // remember it in case it becomes free
//             best = b;
//         }
//         else
//         {
//             // increment free size
//             bsize += psize;
//             // clear this memory block as it will be packed
//             *b = 0;
//             // point to next memory block
//             b += psize >> 1;
//         }
//     }

//     // last free block update
//     if (bsize != 0)
//     {
//         // store packed free size
//         *best = bsize;

//         // return it if greater than what we're looking for
//         if (bsize >= nsize)
//             return best;
//     }

//     return NULL;
// }