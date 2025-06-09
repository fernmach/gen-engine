#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "physics.h"
#include "ecs.h"

#include <genesis.h>

// --- Global component array definitions ---
extern PositionComponent       g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent       g_velocities[ECS_MAX_ENTITIES];
extern SpriteComponent         g_sprites[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
extern bool g_entity_active[ECS_MAX_ENTITIES];
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

void MovementSystem_update(fix16 dt) {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
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

// --- qsort Comparison Function ---
// Compares two PhysicsComponent pointers based on their AABB's 'left' edge.
// qsort expects a function that takes two const void* arguments.
int CollisionSystem_compareBodiesByLeftEdge(const void* a, const void* b) {
    // The arguments are pointers to elements of the array being sorted.
    // In our case, the array s_activeBodyPointers contains PhysicsComponent* elements.
    // So, 'a' and 'b' are effectively PhysicsComponent**.
    RigidBodyComponent* bodyA = *(RigidBodyComponent**)a;
    RigidBodyComponent* bodyB = *(RigidBodyComponent**)b;

    if (bodyA->shape.colliderShape.box.min.x < bodyB->shape.colliderShape.box.min.x) {
        return -1;
    }

    if (bodyA->shape.colliderShape.box.min.x > bodyB->shape.colliderShape.box.min.x) {
        return 1;
    }
    
    return 0;
}

// // Sweep and prune implementation
// void CollisionSystem_update() {
//        // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_RIGID_BODY;

//     u8 activeBodyCount = 0;    
//     // Used for sweep and prune broad phase colligion detction
//     RigidBodyComponent* g_active_body_bointers[ECS_MAX_ENTITIES];

//     // Create the array that will be sorted
//     for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
//         if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {
//             g_active_body_bointers[activeBodyCount++] = &g_rigid_bodies[i];
//         }
//     }

//     //TODO: Is this really necessary?
//     if (activeBodyCount < 2) {
//         return; // Not enough bodies to check for collisions
//     }

//     // 2. Sort active bodies by their left edge (min X-coordinate)
//     //    qsort(array_to_sort, num_elements, size_of_element, comparison_function)
//     qsort(g_active_body_bointers, 
//         activeBodyCount, 
//         sizeof(RigidBodyComponent*), 
//         &CollisionSystem_compareBodiesByLeftEdge);

//     for (EntityId i = 0; i < activeBodyCount; ++i) {
//         RigidBodyComponent* body1 = &g_active_body_bointers[i];

//         AABBColliderShape a;
//         a.min.x = F16_toInt(g_positions[i].x);
//         a.min.y = F16_toInt(g_positions[i].y);
//         a.max.x = F16_toInt(g_positions[i].x) + body1->shape.colliderShape.box.max.x;
//         a.max.y = F16_toInt(g_positions[i].y) + body1->shape.colliderShape.box.max.y;

//         // Check colligion agains all entityes 
//         for (EntityId j = i+1; j < activeBodyCount; ++j) {
//             RigidBodyComponent* body2 = &g_active_body_bointers[j];

//             // Pruning step:
//             // If body2's left edge is beyond body1's right edge,
//             // then no subsequent bodies (which are sorted further to the right)
//             // can overlap with body1 on the X-axis.
//             if (body2->shape.colliderShape.box.min.x > body1->shape.colliderShape.box.min.y) {
//                 break; // Prune this inner loop for body1
//             }

//             AABBColliderShape b;
//             b.min.x = F16_toInt(g_positions[j].x);
//             b.min.y = F16_toInt(g_positions[j].y);
//             b.max.x = F16_toInt(g_positions[j].x) + body2->shape.colliderShape.box.max.x;
//             b.max.y = F16_toInt(g_positions[j].y) + body2->shape.colliderShape.box.max.y;            

//             if ( AABBvsAABB( a, b ) ) {
//                 LOGGER_DEBUG("Collision detected: Entities %d, %d", i, j);
//             }
//         }
//     }    

// }

void CollisionSystem_update() {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_RIGID_BODY;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {

            // Need to take into consideration the sprite position on the screen
            // AABBColliderShape a = {
            //     {
            //         F16_toInt(g_positions[i].x),
            //         F16_toInt(g_positions[i].y)
            //     },
            //     {
            //         F16_toInt(g_positions[i].x) + g_rigid_bodies[i].shape.colliderShape.box.max.x,
            //         F16_toInt(g_positions[i].y) + g_rigid_bodies[i].shape.colliderShape.box.max.y
            //     }
            // };            
            
            AABBColliderShape a;
            a.min.x = F16_toInt(g_positions[i].x);
            a.min.y = F16_toInt(g_positions[i].y);
            a.max.x = F16_toInt(g_positions[i].x) + g_rigid_bodies[i].shape.colliderShape.box.max.x;
            a.max.y = F16_toInt(g_positions[i].y) + g_rigid_bodies[i].shape.colliderShape.box.max.y;
            
            // Check colligion agains all entityes 
            for (EntityId j = i+1; j < ECS_MAX_ENTITIES; ++j) {
                
                //if (i == j)
                  //  continue;

                if (g_entity_active[j] && Entity_hasAllComponents(j, required_mask)) {

                    // // Function to check for collision between two entities (Axis-Aligned Bounding Box)
                    // bool checkCollision(const Entity* entity1, const Entity* entity2) {
                    //     return (entity1->position.x < entity2->position.x + entity2->size.width &&
                    //             entity1->position.x + entity1->size.width > entity2->position.x &&
                    //             entity1->position.y < entity2->position.y + entity2->size.height &&
                    //             entity1->position.y + entity1->size.height > entity2->position.y);
                    // }
                    
                    AABBColliderShape b;
                    b.min.x = F16_toInt(g_positions[j].x);
                    b.min.y = F16_toInt(g_positions[j].y);
                    b.max.x = F16_toInt(g_positions[j].x) + g_rigid_bodies[j].shape.colliderShape.box.max.x;
                    b.max.y = F16_toInt(g_positions[j].y) + g_rigid_bodies[j].shape.colliderShape.box.max.y;

                    // LOGGER_DEBUG("Sprite width: A %d, %d", 
                    //     F16_toInt(g_sprites[i].sgdkSprite->definition->w),
                    //     F16_toInt(g_sprites[i].sgdkSprite->definition->h)
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
                        LOGGER_DEBUG("Collision detected: Entities %d, %d", i, j);
                    }
                }
            }
        }
    }
}

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