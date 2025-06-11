#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "physics.h"
#include "ecs.h"
#include "utils.h"

#include <genesis.h>

#include <tools.h>

// --- Global component array definitions ---
extern PositionComponent       g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent       g_velocities[ECS_MAX_ENTITIES];
extern SpriteComponent         g_sprites[ECS_MAX_ENTITIES];
extern RigidBodyComponent      g_rigid_bodies[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
extern bool g_entity_active[ECS_MAX_ENTITIES];
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// Temporary array to hold pointers to *active* physics components for sorting.
// This is crucial to avoid sorting inactive components or copying large structs.
// Used for sweep and prune broad phase colligion detction
static u16 g_active_rb_count = 0;

//Should be EntityId
static u16 g_active_rb[ECS_MAX_ENTITIES];


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

bool CollisionSystem_compareByLeftEdge(u16 i, u16 j) {
    return g_positions[i].x > g_positions[j].x;    
}

// Sweep and prune
void CollisionSystem_update() {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_RIGID_BODY;

    // Currently active rigid bodies
    g_active_rb_count = 0;

    // Create the array that will be sorted
    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {
            g_active_rb[g_active_rb_count++] = i;            
            //LOGGER_DEBUG("Collision: Active rigid body pos: %d, id: %d", g_active_rb_count, i);
        }
    }

    // Not enough bodies to check for collisions
    if (g_active_rb_count < 2) {
        //LOGGER_DEBUG("Collision: Not enough bodies to check for collisions");
        return;
    }

    //u16 n = sizeof(g_active_rb) / sizeof(g_active_rb[0]);
    insertionSort(g_active_rb, g_active_rb_count, CollisionSystem_compareByLeftEdge);

    //LOGGER_DEBUG('ORDERED BODIES');
    //printArray(g_active_rb, g_active_rb_count);

    for (EntityId i = 0; i < g_active_rb_count; ++i) {
        // Use the first entityId from the ordered list to get the doby for testing
        EntityId body1Id = g_active_rb[i];
        RigidBodyComponent body1 = g_rigid_bodies[ body1Id ];
        //LOGGER_DEBUG("BODY 1 id: %d", g_active_rb[i]);

        // Check colligion agains all entityes 
        for (EntityId j = i+1; j < g_active_rb_count; ++j) {
            EntityId body2Id = g_active_rb[j];
            RigidBodyComponent body2 = g_rigid_bodies[ body2Id ];

            // LOGGER_DEBUG("BODY 2 id: %d", g_active_rb[j]);

            // LOGGER_DEBUG("Checking positions: Body 2 %d, Body 1 %d",
            //     F16_toInt(g_positions[j].x),
            //     F16_toInt(g_positions[i].x) + body1.collider.shape.box.max.x
            // );

            // Pruning step:
            // If body2's left edge is beyond body1's right edge,
            // then no subsequent bodies (which are sorted further to the right)
            // can overlap with body1 on the X-axis.
            //if (body2->shape.colliderShape.box.min.x > body1->shape.colliderShape.box.max.x) {
            if ( F16_toInt(g_positions[body2Id].x) >
                 F16_toInt(g_positions[body1Id].x) + body1.collider.shape.box.max.x
            ) {
                //LOGGER_DEBUG("Collision: Prunning BODY 1 - %d, BODY 2 - %d", body1Id, body2Id);
                break; // Prune this inner loop for body1->
            } 
            // else {
            //     LOGGER_DEBUG("Not prunning need to check %d, %d",
            //         F16_toInt(g_positions[body2Id].x),
            //         F16_toInt(g_positions[body1Id].x) + body1.collider.shape.box.max.x
            //     );
            // }

            AABBColliderShape a;
            a.min.x = F16_toInt(g_positions[body1Id].x);
            a.min.y = F16_toInt(g_positions[body1Id].y);
            a.max.x = F16_toInt(g_positions[body1Id].x) + body1.collider.shape.box.max.x;
            a.max.y = F16_toInt(g_positions[body1Id].y) + body1.collider.shape.box.max.y;

            AABBColliderShape b;
            b.min.x = F16_toInt(g_positions[body2Id].x);
            b.min.y = F16_toInt(g_positions[body2Id].y);
            b.max.x = F16_toInt(g_positions[body2Id].x) + body2.collider.shape.box.max.x;
            b.max.y = F16_toInt(g_positions[body2Id].y) + body2.collider.shape.box.max.y;

            // LOGGER_DEBUG("Sprite width: A %d, %d", 
            //     g_sprites[i].sgdkSprite->definition->w,
            //     g_sprites[i].sgdkSprite->definition->h
            // );

            // LOGGER_DEBUG("Position: A %d, %d -- B %d, %d", 
            //     F16_toInt(g_positions[i].x), F16_toInt(g_positions[i].y),
            //     F16_toInt(g_positions[j].x), F16_toInt(g_positions[j].y));

            // LOGGER_DEBUG("A -- Min %d, %d -- Max %d, %d", 
            //     a.min.x, a.min.y,
            //     a.max.x, a.max.y);

            // LOGGER_DEBUG("B -- Min %d, %d -- Max %d, %d", 
            //     b.min.x, b.min.y,
            //     b.max.x, b.max.y);

            //ASSERT_EXP(1==0);

            if ( AABBvsAABB( a, b ) ) {
                LOGGER_DEBUG("Collision detected: Entities %d, %d", body1Id, body2Id);
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
//                         LOGGER_DEBUG("Collision detected: Entities %d, %d", i, j);
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

