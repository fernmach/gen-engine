#ifndef _ENG_FSM_H_
#define _ENG_FSM_H_

#include "defines.h"
#include <genesis.h>

// Forward declaration to avoid circular dependencies with your ECS header
//struct FSMComponent;

// A generic State Function pointer. It takes the entity ID as an argument
// so the function can access any of the entity's other components.
typedef void (*PFN_State)(EntityId entityId);

// Defines the logic for a single state.
// You can use NULL for functions that are not needed.
typedef struct {
    PFN_State onEnter;  // Called once when entering the state
    PFN_State onUpdate; // Called every frame while in the state
    PFN_State onExit;   // Called once when exiting the state
} State;

// Holds the array of all possible states for a specific FSM type (e.g., Player FSM)
// This is usually a 'const' global structure.
typedef struct {
    const State* states;
    const u16 numStates;
} FSMDefinition;

// --- System Functions ---

/**
 * @brief Changes the state of an entity's FSM.
 * This is the primary way to trigger state transitions.
 * It handles calling onExit for the old state and onEnter for the new one.
 * 
 * @param entityId The ID of the entity to change.
 * @param newState The new state to transition to.
 */
void FSM_changeState(EntityId entityId, u16 newState);

/**
 * @brief The main FSM update system.
 * Call this once per frame in your game loop.
 * It iterates through all entities with an FSMComponent and calls their
 * current state's onUpdate function.
 */
//void FSM_updateSystem(void);


#endif // FSM_H