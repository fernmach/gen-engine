#include "fsm.h"
#include "ecs.h"

void FSM_changeState(EntityId entityId, u16 newState) {
    // Get the FSM component for the entity
    FSMComponent* fsm = Entity_getComponentFSM(entityId); // <--- REPLACE WITH YOUR ECS FUNCTION

    // Don't transition to the same state
    if (fsm->currentState == newState) {
        return;
    }
    
    // 1. Call the onExit function of the OLD state, if it exists
    const State* oldStateLogic = &fsm->definition->states[fsm->currentState];
    if (oldStateLogic->onExit != NULL) {
        oldStateLogic->onExit(entityId);
    }

    // 2. Update to the new state
    fsm->currentState = newState;
    fsm->stateTimer = 0; // Reset the timer on state change

    // 3. Call the onEnter function of the NEW state, if it exists
    const State* newStateLogic = &fsm->definition->states[fsm->currentState];
    if (newStateLogic->onEnter != NULL) {
        newStateLogic->onEnter(entityId);
    }
}

// void FSM_updateSystem() {
//      // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_FSM;

//     // This loop depends on your ECS implementation.
//     // It should iterate over all entities that have an FSMComponent.
//     // For this example, let's assume a simple loop.
//     for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; i++) {
//         // Assume your ECS has a way to check if an entity has a component
//         if (Entity_hasAllComponents(i, required_mask)) {
//             FSMComponent* fsm = Entity_getComponentFSMRef(i);

//             // Update the state timer
//             if(fsm->stateTimer > 0) {
//                 fsm->stateTimer--;
//             }

//             // Call the onUpdate function for the current state, if it exists
//             const State* currentStateLogic = &fsm->definition->states[fsm->currentState];
//             if (currentStateLogic->onUpdate != NULL) {
//                 currentStateLogic->onUpdate(i);
//             }
//         }
//     }
// }