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