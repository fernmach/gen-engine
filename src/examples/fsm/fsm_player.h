#ifndef _FSM_EXAMPLE_PLAYER_H_
#define _FSM_EXAMPLE_PLAYER_H_

#include "../../core/genengine.h"

// Define the specific states for the Player.
// Using an enum makes the code much more readable.
typedef enum {
    PLAYER_STATE_IDLE,
    PLAYER_STATE_WALKING,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_ATTACKING
} PlayerState;

// Declare the global definition for the player's FSM
extern const FSMDefinition playerFSMDefinition;

// A function to create a new player entity
EntityId Player_create(s16 x, s16 y);

#endif // PLAYER_H