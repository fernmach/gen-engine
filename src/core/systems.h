#ifndef _ENG_SYSTEMS_H_
#define _ENG_SYSTEMS_H_

#include "defines.h"
#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"
#include "utils.h"
//#include "vector_math.h"
#include "debug.h"
#include "event.h"
#include "systems.h"

#include <genesis.h>

typedef struct {
    EntityId entityIdA;
    EntityId entityIdB;
} ContactPair;

// --- Global component array definitions ---
extern PositionComponent    g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent    g_velocities[ECS_MAX_ENTITIES];
extern ColliderComponent    g_colliders[ECS_MAX_ENTITIES];
extern SpriteComponent      g_sprites[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// Temporary array to hold pointers to *active* physics components for sorting.
// This is crucial to avoid sorting inactive components or copying large structs.
// Used for sweep and prune broad phase colligion detction
static u16 g_active_colliders_count = 0;

//Should be EntityId
static u16 g_active_colliders[ECS_MAX_ENTITIES];

//static ContactPair g_contact_pairs[ECS_MAX_ENTITIES];
//static u16 g_contact_pairs_count = 0;

// Evaluate if the viual collider debugging is enabled
#if DEBUG_COLLIDER_BORDERS_ENABLED

static inline void DebugSystem_clearColliderBoxUpdate() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_COLLIDER;

    // Create the array that will be sorted
    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {                        
            const ColliderComponent* collider = &g_colliders[i];
            PositionComponent* position = &g_positions[i];
            DBG_CLEAR_COLLIDER_BOX(collider, position);
        }
    }    
}

static inline void DebugSystem_drawColliderBoxUpdate() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_COLLIDER;

    // Create the array that will be sorted
    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {                        
            const ColliderComponent* collider = &g_colliders[i];
            PositionComponent* position = &g_positions[i];
            DBG_DRAW_COLLIDER_BOX(collider, position);
        }
    }    
}

#endif

static inline void MovementSystem_update(fix16 dt) {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

    // This version is significantly faster because it eliminates the repeated, expensive 
    // base + index * size calculations inside the loop's core logic.
    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        //RenderSystemDebug_clearText(i);
        if (Entity_hasAllComponents(i, required_mask)) {
            PositionComponent* const pPos = &g_positions[i];
            const VelocityComponent* pVel = &g_velocities[i];
            pPos->x += F16_mul(pVel->dx, dt);
            pPos->y += F16_mul(pVel->dy, dt);            
            //LOGGER_DEBUG("Running %d %d", g_positions[i].x, g_positions[i].y);
        }
    }
}

static inline void ScreenConstraintSystem_update() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE |COMPONENT_SCREEN_CONSTRAINT;

    const u16 screen_width = VDP_getScreenWidth();
    const u16 screen_height = VDP_getScreenHeight();    

    //LOGGER_DEBUG("%d, %d", screen_width, screen_height);W

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {

            // ... (screen bounds logic) ...
            //s16 screen_x = F16_toInt(g_positions[i].x);
            //s16 screen_y = F16_toInt(g_positions[i].y);

            PositionComponent* const pPos = &g_positions[i];
            VelocityComponent* const pVel = &g_velocities[i];
            Sprite* const pSprite = g_sprites[i].sgdkSprite;
            
            //screen x
            if (F16_toInt(g_positions[i].x) < 0) {                
                pPos->x = FIX16(0);
                pVel->dx = -pVel->dx;                
            }
            
            //screen y
            if (F16_toInt(pPos->y) < 0) {                
                pPos->y = FIX16(0);
                pVel->dy = -pVel->dy;
            }

            if ((F16_toInt(pPos->x) + pSprite->definition->w) > screen_width) {                    
                pPos->x = FIX16(screen_width - pSprite->definition->w);                    
                pVel->dx = -pVel->dx;                    
            }

            if ((F16_toInt(pPos->y) + pSprite->definition->h) > screen_height) {
                pPos->y = FIX16(screen_height - pSprite->definition->h);
                pVel->dy = -pVel->dy;
            }
        }
    }
}

static inline bool CollisionSystem_compareByLeftEdge(u16 i, u16 j) {
    return g_positions[i].x > g_positions[j].x;    
}

// Pass pointers directly! Avoids lookups inside the function.
static inline bool CollisionSystem_checkForCollision(      
    const PositionComponent* pPosA, const ColliderComponent* pCollA,
    const PositionComponent* pPosB, const ColliderComponent* pCollB,
    fix16* out_penetration, Vect2D_f16* out_normal 
) {
    // Calculate AABB bounds from the passed-in data.
    // It's assumed pos is the top-left of the entity, and the collider's x/y is an offset.    
    fix16 a_min_x = pPosA->x + FIX16(pCollA->shape.box.x);
    fix16 a_min_y = pPosA->y + FIX16(pCollA->shape.box.y);
    fix16 a_w = FIX16(pCollA->shape.box.w);
    fix16 a_h = FIX16(pCollA->shape.box.h);

    fix16 b_min_x = pPosB->x + FIX16(pCollB->shape.box.x);
    fix16 b_min_y = pPosB->y + FIX16(pCollB->shape.box.y);
    fix16 b_w = FIX16(pCollB->shape.box.w);
    fix16 b_h = FIX16(pCollB->shape.box.h);
    
    // 2. Calculate center points and half-extents (half-widths/heights)
    // use << 1 instead?
    //fix16 a_half_w = F16_div(a_w, FIX16(2));
    //fix16 a_half_h = F16_div(a_h, FIX16(2));
    fix16 a_half_w = F16_half(a_w);
    fix16 a_half_h = F16_half(a_h);
    Vect2D_f16 centerA = { a_min_x + a_half_w, a_min_y + a_half_h };

    // fix16 b_half_w = F16_div(b_w, FIX16(2));
    // fix16 b_half_h = F16_div(b_h, FIX16(2));
    fix16 b_half_w = F16_half(b_w);
    fix16 b_half_h = F16_half(b_h);    
    Vect2D_f16 centerB = { b_min_x + b_half_w, b_min_y + b_half_h };

    // 3. Calculate the vector from the center of A to the center of B
    Vect2D_f16 n = { centerB.x - centerA.x, centerB.y - centerA.y };

    // 4. Calculate the extent of overlap on each axis
    fix16 x_overlap = (a_half_w + b_half_w) - F16_abs(n.x);
    fix16 y_overlap = (a_half_h + b_half_h) - F16_abs(n.y);

    // 5. If there is no overlap on either axis, there is no collision
    if (x_overlap <= 0 || y_overlap <= 0) {
        return false;
    }

    // 6. Find the axis of minimum penetration
    if (x_overlap < y_overlap) {
        // Pointing right if n.x > 0, left if n.x < 0
        *out_penetration = x_overlap;
        out_normal->x = (n.x < 0) ? FIX16(-1) : FIX16(1);
        out_normal->y = FIX16(0);
    } else {
        // Pointing down if n.y > 0, up if n.y < 0
        *out_penetration = y_overlap;
        out_normal->x = FIX16(0);
        out_normal->y = (n.y < 0) ? FIX16(-1) : FIX16(1);
    }

    return true;
}

static inline void CollisionSystem_resolveCollision(
    // Pass pointers to the data that will be modified/read.
    PositionComponent* pPosA, const ColliderComponent* pCollA,
    PositionComponent* pPosB, const ColliderComponent* pCollB,
    fix16 penetration, Vect2D_f16 normal
) {
    // Collider 2 is static (move A out of B)
    if (pCollB->isStatic) {
        pPosA->x -= F16_mul(normal.x, penetration);
        pPosA->y -= F16_mul(normal.y, penetration);
    } 
    // Collider 1 is static (move B out of A)
    else if (pCollA->isStatic) {                   
        // OPTIMIZATION: Cleaner negation.
        pPosB->x -= F16_mul(-normal.x, penetration);
        pPosB->y -= F16_mul(-normal.y, penetration);
    } 
    // Neither is static, move both by half
    else {
        // OPTIMIZATION: Use fast bit-shift instead of slow division.
        fix16 half_pen = F16_half(penetration);
        //fix16 half_pen = F16_div(penetration, FIX16(2));
        pPosA->x -= F16_mul(normal.x, half_pen);
        pPosA->y -= F16_mul(normal.y, half_pen);
        pPosB->x += F16_mul(normal.x, half_pen);
        pPosB->y += F16_mul(normal.y, half_pen);
    }
}

// Sweep and prune
// ALL IN ONE IMPLEMENTATION
static inline void CollisionSystem_update() {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_COLLIDER;

    fix16 penetration;
    Vect2D_f16 normal;

    // Currently active rigid bodies
    g_active_colliders_count = 0;

    // Create the array that will be sorted
    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {
            g_active_colliders[g_active_colliders_count++] = i;            
            //LOGGER_DEBUG("Collision: Active rigid body pos: %d, id: %d", g_active_colliders_count, i);
        }
    }

    // Not enough bodies to check for collisions
    if (g_active_colliders_count < 2) {
        //LOGGER_DEBUG("Collision: Not enough bodies to check for collisions");
        return;
    }

    //Order    
    insertionSort(g_active_colliders,
        g_active_colliders_count,
        CollisionSystem_compareByLeftEdge);
    //LOGGER_DEBUG('ORDERED BODIES');
    //printArray(g_active_rb, g_active_rb_count);

    //Sweep
    for (EntityId i = 0; i < g_active_colliders_count - 1; ++i) {

        // Use the first entityId from the ordered list to get the doby for testing
        const EntityId collider1Id = g_active_colliders[i];
        const ColliderComponent* pCollider1 = &g_colliders[collider1Id];
        PositionComponent* pPosition1 = &g_positions[collider1Id];

        // This value only depends on `i`, not `j`. Calculate it ONCE here.
        //s32???
        const s16 body1_right_edge = F16_toInt(pPosition1->x) + 
                                     (s16)(pCollider1->shape.box.x + pCollider1->shape.box.w);
        
        // Check colligion agains all entities and break if prune
        for (EntityId j = i+1; j < g_active_colliders_count; ++j) {

            // Use the second entityId from the ordered list to get the doby for testing
            // Use pointers for the second body as well.
            const EntityId collider2Id = g_active_colliders[j];
            const ColliderComponent* pCollider2 = &g_colliders[collider2Id];
            PositionComponent* pPosition2 = &g_positions[collider2Id];

            // Do not attempt to check collisions when both are static bodies.
            if (pCollider1->isStatic && pCollider2->isStatic) {
                //LOGGER_DEBUG("Collision: Skipping static bodies: BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
                continue;
            }

            // Check if the colliders are in the same layer, and has the same mask
            if (((pCollider1->layer & pCollider2->iteractWith) == 0) || ((pCollider2->layer & pCollider1->iteractWith) == 0)) {
                //LOGGER_DEBUG("Collision: Skipping DIFFERENT layers: BODYIES %d, %d", collider1Id, collider2Id);
                continue; // 'break' was likely a bug here too.
            }

            // Pruning step: Use the pre-calculated value.
            if (F16_toInt(pPosition2->x) > body1_right_edge) {
                // LOGGER_DEBUG("Collision: Prunning BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
                break; // This break is correct. No further bodies can collide with body1.
            }

            //LOGGER_DEBUG("Collision: BODIES %d,%d - LAYERS %d,%d - MASKS: %d,%d", collider1Id, collider2Id, collider1.layer, collider2.layer, collider1.iteractWith, collider2.iteractWith);

            if(CollisionSystem_checkForCollision(
                pPosition1, pCollider1,
                pPosition2, pCollider2,
                &penetration, &normal) 
            ) {
                // LOGGER_DEBUG("COLLIDED %d, %d, pen: %d normal: %d, %d, static, %d, %d", collider1Id, collider2Id, penetration, normal.x, normal.y, g_colliders[collider1Id].isStatic, g_colliders[collider2Id].isStatic);
                
                // The collision resolution will not happen over triggers.
                // only the event will happen
                if(!pCollider1->isTrigger && !pCollider2->isTrigger) {
                    
                    // Resolve collision
                    CollisionSystem_resolveCollision(
                        pPosition1, pCollider1,
                        pPosition2, pCollider2,
                        penetration, normal
                    );
                }

                // Raise the collision event
                Event collisionEvent = {
                    .type = EVT_COLLISION,
                    .data.collision = {
                        .entityA = collider1Id,
                        .entityB = collider2Id,
                        .normal = normal,
                        .penetration = penetration
                    }
                };
                
                // Publish collision event
                Event_publish(&collisionEvent);                
            }
        }
    }
}

static inline void FSMSystem_update() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_FSM;

    // This loop depends on your ECS implementation.
    // It should iterate over all entities that have an FSMComponent.
    // For this example, let's assume a simple loop.
    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; i++) {
        // Assume your ECS has a way to check if an entity has a component
        if (Entity_hasAllComponents(i, required_mask)) {
            FSMComponent* fsm = Entity_getComponentFSM(i);            

            // Update the state timer
            if(fsm->stateTimer > 0) {
                fsm->stateTimer--;
            }

            // Call the onUpdate function for the current state, if it exists
            const State* currentStateLogic = &fsm->definition->states[fsm->currentState];
            if (currentStateLogic->onUpdate != NULL) {
                currentStateLogic->onUpdate(i);
            }
        }
    }
}

static inline void RenderSystem_update() {
    // This system needs Position (to know where) and Sprite (to know what)
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE;

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {

            Sprite* const pSprite = g_sprites[i].sgdkSprite;

            if (pSprite != NULL) { // Still good to check the actual sprite pointer

                const PositionComponent* pPos = &g_positions[i];

                //LOGGER_DEBUG("Entity %d position {%d, %d}", i, g_positions[i].x, g_positions[i].y);
                //LOGGER_DEBUG("Entity %d screen {%d, %d}", i, screen_x, screen_y);

                // Using F16_toInt would be the correct way to go but the numbers
                // are beeing incorrectly retured.
                s16 screen_x = F16_toInt(pPos->x);
                s16 screen_y = F16_toInt(pPos->y);

                //DBG_CLEAR_SPRITE_BOX(pSprite);
                SPR_setPosition(pSprite, screen_x, screen_y);
                //DBG_DRAW_SPRITE_BOX(pSprite);

                // ... other sprite updates ...
                // Update animation, visibility, etc. based on SpriteComponent data
                // SPR_setAnim(g_sprites[i].sgdk_sprite, g_sprites[i].current_anim_frame);
                // SPR_setVisibility(g_sprites[i].sgdk_sprite, VISIBLE / HIDDEN);
            }
        }
    }
}

#endif // SYSTEMS_H
