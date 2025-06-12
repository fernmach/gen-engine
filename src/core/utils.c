#include "utils.h"

#include <genesis.h>
#include <types.h>

static u16 g_random_number_counter = 0;

u16 getRandomNumberInRange( u16 min, u16 max ) {
    setRandomSeed( F16_toInt ( F32_toFix16( getTick() ) ) + g_random_number_counter );
    
    g_random_number_counter++;
    if (g_random_number_counter == MAX_U16) {
        g_random_number_counter = 0;
    }

    return ((random() % (max - min + 1)) + min);
    //LOGGER_DEBUG("Random: %u", randomNumber);
    //u16 randomNumber = random() % 200 + 1;
    //return randomNumber;
    // int min = 10; // Replace with your desired minimum value
    // int max = 100; // Replace with your desired maximum value
    // int randomNumber = (rand() % (max - min + 1)) + min;
}

void insertionSort(u16 arr[], u16 n, PFN_compareCallback compare) {
    for (u16 i = 1; i < n; i++) {
        u16 key = arr[i];
        s16 j = i - 1; // Needs to be signed
        // Here, we call the provided comparison function 'cmp'
        while (j >= 0 && compare(arr[j], key)) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

//TODO: Is is really necessary for better performance?
#define INSORT(type, signed_type) \
void insertionSort_##type(type arr[], type n, PFN_compareCallback compare) {  \
    for (type i = 1; i < n; i++) {                                            \
        type key = arr[i];                                                    \
        signed_type j = i - 1;                                                \
        while (j >= 0 && compare(arr[j], key)) {                              \
            arr[j + 1] = arr[j];                                              \
            j = j - 1;                                                        \
        }                                                                     \
        arr[j + 1] = key;                                                     \
    }                                                                         \
}                                                                             \

INSORT(u8, s16);
INSORT(u16, s16);
//INSORT(EntityId, s16);

// TODO: INCLUDE TESTES IN THEIR OWN FILE
// COMMENT IF NOT USING
// ----------------TESTS-----------------//

// // Callback function to sort in ascending order.
// // Returns true (1) if the first element is greater than the second.
// bool ascending(u16 a, u16 b) {
//     return a > b;
// }

// // Callback function to sort in descending order.
// // Returns true (1) if the first element is less than the second.
// bool descending(u16 a, u16 b) {
//     return a < b;
// }

// // Helper function to print an array
// void printArray(u16 arr[], u16 n) {
//     for (u16 i = 0; i < n; i++) {
//         KLog_U1(" ", arr[i]);
//     }
// }

// void insertSort_TEST() {
//     u16 original_arr[] = {64, 34, 25, 12, 22, 11, 90};
//     u16 n = sizeof(original_arr) / sizeof(original_arr[0]);

//      // Create a copy to sort ascending
//     u16 arr_asc[n];
//     memcpy(arr_asc, original_arr, n * sizeof(int));

//     KLog("Original array: ");
//     printArray(arr_asc, n);

//     // Sort in ascending order by passing the 'ascending' function
//     insertionSort(arr_asc, n, ascending);
//     KLog("Sorted array (Ascending): ");
//     printArray(arr_asc, n);

//     // Create another copy to sort descending
//     u16 arr_desc[n];
//     memcpy(arr_desc, original_arr, n * sizeof(int));

//     // Sort in descending order by passing the 'descending' function
//     insertionSort(arr_desc, n, descending);
//     KLog("Sorted array (Descending): ");
//     printArray(arr_desc, n);

//     return;
// }

// ----------------END TESTS-----------------//