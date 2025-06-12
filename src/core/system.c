#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"
#include "utils.h"

#include <genesis.h>

// --- Global component array definitions ---
extern PositionComponent    g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent    g_velocities[ECS_MAX_ENTITIES];
extern ColliderComponent    g_colliders[ECS_MAX_ENTITIES];
extern SpriteComponent      g_sprites[ECS_MAX_ENTITIES];


// --- Global entity tracking array definitions  ---
extern bool g_entity_active[ECS_MAX_ENTITIES];
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// Temporary array to hold pointers to *active* physics components for sorting.
// This is crucial to avoid sorting inactive components or copying large structs.
// Used for sweep and prune broad phase colligion detction
static u16 g_active_colliders_count = 0;

//Should be EntityId
static u16 g_active_colliders[ECS_MAX_ENTITIES];

void MovementSystem_update(fix16 dt) {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        //RenderSystemDebug_clearText(i);
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {            
            g_positions[i].x += F16_mul(g_velocities[i].dx, dt);
            g_positions[i].y += F16_mul(g_velocities[i].dy, dt);            
            //LOGGER_DEBUG("Running %d %d", g_positions[i].x, g_positions[i].y);
        }
    }
}

void ScreenConstraintSystem_update() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE |COMPONENT_SCREEN_CONSTRAINT;

    const u16 screen_width = VDP_getScreenWidth();
    const u16 screen_height = VDP_getScreenHeight();
    //const u16 screen_width = 320;
    //const u16 screen_height = 224;

    //LOGGER_DEBUG("%d, %d", screen_width, screen_height);

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {

            // ... (screen bounds logic) ...
            //s16 screen_x = F16_toInt(g_positions[i].x);
            //s16 screen_y = F16_toInt(g_positions[i].y);
            
            //screen x
            if (F16_toInt(g_positions[i].x) < 0) {                
                g_positions[i].x = FIX16(0);
                g_velocities[i].dx = -g_velocities[i].dx;                
            }
            
            //screen y
            if (F16_toInt(g_positions[i].y) < 0) {                
                g_positions[i].y = FIX16(0);
                g_velocities[i].dy = -g_velocities[i].dy;
            }

            if ((F16_toInt(g_positions[i].x) + g_sprites[i].sgdkSprite->definition->w) > screen_width) {                    
                g_positions[i].x = FIX16(screen_width - g_sprites[i].sgdkSprite->definition->w);                    
                g_velocities[i].dx = -g_velocities[i].dx;                    
            }

            if ((F16_toInt(g_positions[i].y) + g_sprites[i].sgdkSprite->definition->h) > screen_height) {
                g_positions[i].y = FIX16(screen_height - g_sprites[i].sgdkSprite->definition->h);
                g_velocities[i].dy = -g_velocities[i].dy;
            }
        }
    }
}

bool CollisionSystem_AABBvsAABB(EntityId aId, EntityId bId ) {
    // Exit with no intersection if found separated along an axis
    PositionComponent posA = g_positions[aId];
    ColliderComponent collA = g_colliders[aId];

    PositionComponent posB = g_positions[bId];
    ColliderComponent collB = g_colliders[bId];

    fix16 a_min_left = posA.x;
    fix16 a_min_right = posA.y;
    fix16 a_max_left = posA.x + FIX16(collA.shape.box.x + collA.shape.box.w);
    fix16 a_max_right = posA.y + FIX16(collA.shape.box.y + collA.shape.box.h);

    fix16 b_min_left = posB.x;
    fix16 b_min_right = posB.y;
    fix16 b_max_left = posB.x + FIX16(collB.shape.box.x + collB.shape.box.w);
    fix16 b_max_right = posB.y + FIX16(collB.shape.box.y + collB.shape.box.h);  

    //if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if(a_max_left < b_min_left || a_min_left > b_max_left) return false;
    if(a_max_right < b_min_right || a_min_right > b_max_right) return false;
    // No separating axis found, therefor there is at least one overlapping axis 
    return true;    
}

bool CollisionSystem_compareByLeftEdge(u16 i, u16 j) {
    return g_positions[i].x > g_positions[j].x;    
}

// Sweep and prune
void CollisionSystem_update() {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_COLLIDER;

    // Currently active rigid bodies
    g_active_colliders_count = 0;

    // Create the array that will be sorted
    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {
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
    for (EntityId i = 0; i < g_active_colliders_count; ++i) {

        // Use the first entityId from the ordered list to get the doby for testing
        EntityId collider1Id = g_active_colliders[i];
        ColliderComponent collider1 = g_colliders[ collider1Id ];
        
        // Check colligion agains all entities and break if prune
        for (EntityId j = i+1; j < g_active_colliders_count; ++j) {
            EntityId collider2Id = g_active_colliders[j];            
            
            // Pruning step:
            // If body2's left edge is beyond body1's right edge,
            // then no subsequent bodies (which are sorted further to the right)
            // can overlap with body1 on the X-axis.
            //if (body2->shape.colliderShape.box.min.x > body1->shape.colliderShape.box.max.x) {            
            if ( F16_toInt(g_positions[collider2Id].x) >
                 F16_toInt(g_positions[collider1Id].x) + 
                 (s16)(collider1.shape.box.x + collider1.shape.box.w)
            ) {
                //LOGGER_DEBUG("Collision: Prunning BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
                break; // Prune this inner loop for body1->
            }           

            //ASSERT_EXP(1==0);
            if(CollisionSystem_AABBvsAABB(collider1Id, collider2Id)) {
                LOGGER_DEBUG("Collision: Dectected colligion beween %d && %d", collider1Id, collider2Id);
            }
        }
    }
}

//Brute force (working properly);
// void CollisionSystem_update() {
//     // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_RIGID_BODY;

//     for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
//         if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {

//             // Need to take into consideration the sprite position on the screen
//             // AABBColliderShape a = {
//             //     {
//             //         F16_toInt(g_positions[i].x),
//             //         F16_toInt(g_positions[i].y)
//             //     },
//             //     {
//             //         F16_toInt(g_positions[i].x) + g_rigid_bodies[i].shape.colliderShape.box.max.x,
//             //         F16_toInt(g_positions[i].y) + g_rigid_bodies[i].shape.colliderShape.box.max.y
//             //     }
//             // };            
            
//             AABBColliderShape a;
//             a.min.x = F16_toInt(g_positions[i].x);
//             a.min.y = F16_toInt(g_positions[i].y);
//             a.max.x = F16_toInt(g_positions[i].x) + g_rigid_bodies[i].collider.shape.box.max.x;
//             a.max.y = F16_toInt(g_positions[i].y) + g_rigid_bodies[i].collider.shape.box.max.y;
            
//             // Check colligion agains all entityes 
//             for (EntityId j = i+1; j < ECS_MAX_ENTITIES; ++j) {
                
//                 //if (i == j)
//                   //  continue;

//                 if (g_entity_active[j] && Entity_hasAllComponents(j, required_mask)) {

//                     // // Function to check for collision between two entities (Axis-Aligned Bounding Box)
//                     // bool checkCollision(const Entity* entity1, const Entity* entity2) {
//                     //     return (entity1->position.x < entity2->position.x + entity2->size.width &&
//                     //             entity1->position.x + entity1->size.width > entity2->position.x &&
//                     //             entity1->position.y < entity2->position.y + entity2->size.height &&
//                     //             entity1->position.y + entity1->size.height > entity2->position.y);
//                     // }
                    
//                     AABBColliderShape b;
//                     b.min.x = F16_toInt(g_positions[j].x);
//                     b.min.y = F16_toInt(g_positions[j].y);
//                     b.max.x = F16_toInt(g_positions[j].x) + g_rigid_bodies[j].collider.shape.box.max.x;
//                     b.max.y = F16_toInt(g_positions[j].y) + g_rigid_bodies[j].collider.shape.box.max.x;

//                     // LOGGER_DEBUG("Sprite width: A %d, %d", 
//                     //     F16_toInt(g_sprites[i].sgdkSprite->definition->w),
//                     //     F16_toInt(g_sprites[i].sgdkSprite->definition->h)
//                     // );

//                     // LOGGER_DEBUG("Position: A %d, %d -- B %d, %d", 
//                     //     F16_toInt(g_positions[i].x), F16_toInt(g_positions[i].y),
//                     //     F16_toInt(g_positions[j].x), F16_toInt(g_positions[j].y));

//                     // LOGGER_DEBUG("A -- Min %d, %d -- Max %d, %d", 
//                     //     a.min.x, a.min.y,
//                     //     a.max.x, a.max.y);

//                     // LOGGER_DEBUG("B -- Min %d, %d -- Max %d, %d", 
//                     //     b.min.x, b.min.y,
//                     //     b.max.x, b.max.y);

//                     //ASSERT_EXP(1==0);

//                     if ( AABBvsAABB( a, b ) ) {
//                         //LOGGER_DEBUG("Collision detected: Entities %d, %d", i, j);
//                     }
//                 }
//             }
//         }
//     }
// }

void RenderSystem_update() {
    // This system needs Position (to know where) and Sprite (to know what)
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {
            if (g_sprites[i].sgdkSprite != NULL) { // Still good to check the actual sprite pointer

                //LOGGER_DEBUG("Entity %d position {%d, %d}", i, g_positions[i].x, g_positions[i].y);
                //LOGGER_DEBUG("Entity %d screen {%d, %d}", i, screen_x, screen_y);

                // Using F16_toInt would be the correct way to go but the numbers
                // are beeing incorrectly retured.
                s16 screen_x = F16_toInt(g_positions[i].x);
                s16 screen_y = F16_toInt(g_positions[i].y);

                SPR_setPosition(g_sprites[i].sgdkSprite, screen_x, screen_y);        
                // ... other sprite updates ...

                // Update animation, visibility, etc. based on SpriteComponent data
                // SPR_setAnim(g_sprites[i].sgdk_sprite, g_sprites[i].current_anim_frame);
                // SPR_setVisibility(g_sprites[i].sgdk_sprite, VISIBLE / HIDDEN);
            }
        }
    }
}

