/**
 *  \file utils.h
 *  \brief Utility functions
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Provides global generic utility functions.
 */

#ifndef _ENG_UTILS_H_
#define _ENG_UTILS_H_

#include <genesis.h>

// Define a function pointer type for our comparison callback.
typedef bool (*PFN_compareCallback)(u16, u16);

// Get a random number between the specified range
u16 getRandomNumberInRange( u16 min, u16 max );

// Order an array using insertionSort using a compare callback function
// TODO: Figure out how quicksort works in SGDK and use it instead.
void insertionSort(u16 arr[], u16 n, PFN_compareCallback compare);

// TODO: INCLUDE TESTES IN THEIR OWN FILE
// COMMENT IF NOT USING
// ------------------ TESTS --------------------
// Callback function to sort in ascending order.
// Returns true (1) if the first element is greater than the second.
// bool ascending(u16 a, u16 b);
// bool descending(u16 a, u16 b);
// void printArray(u16 arr[], u16 n);
// void insertSort_TEST();

// ------------------ END TESTS --------------------

#endif // _ENG_UTILS_H_