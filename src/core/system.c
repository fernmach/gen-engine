#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"
#include "utils.h"
#include "vector_math.h"
#include "event.h"

#include <genesis.h>

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

void MovementSystem_update(fix16 dt) {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        //RenderSystemDebug_clearText(i);
        if (Entity_hasAllComponents(i, required_mask)) {
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

    //LOGGER_DEBUG("%d, %d", screen_width, screen_height);W

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {

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

//https://code.tutsplus.com/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331t
//AABB collision
bool CollisionSystem_checkForCollision( EntityId aId, EntityId bId, 
    fix16* out_penetration, Vect2D_f16* out_normal )
{
    // 1. Get components from the ECS
    PositionComponent posA = g_positions[aId];
    ColliderComponent collA = g_colliders[aId];

    PositionComponent posB = g_positions[bId];
    ColliderComponent collB = g_colliders[bId];

    // Note: Correctly calculating the AABB bounds. 
    // It's assumed pos is the top-left of the entity, and the collider's x/y is an offset.
    fix16 a_min_x = posA.x + FIX16(collA.shape.box.x);
    fix16 a_min_y = posA.y + FIX16(collA.shape.box.y);
    fix16 a_w = FIX16(collA.shape.box.w);
    fix16 a_h = FIX16(collA.shape.box.h);

    fix16 b_min_x = posB.x + FIX16(collB.shape.box.x);
    fix16 b_min_y = posB.y + FIX16(collB.shape.box.y);
    fix16 b_w = FIX16(collB.shape.box.w);
    fix16 b_h = FIX16(collB.shape.box.h);
    
    // 2. Calculate center points and half-extents (half-widths/heights)
    fix16 a_half_w = F16_div(a_w, FIX16(2));
    fix16 a_half_h = F16_div(a_h, FIX16(2));
    Vect2D_f16 centerA = { a_min_x + a_half_w, a_min_y + a_half_h };

    fix16 b_half_w = F16_div(b_w, FIX16(2));
    fix16 b_half_h = F16_div(b_h, FIX16(2));
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

void CollisionSystem_resolveCollision( EntityId aId, EntityId bId, fix16 penetration, Vect2D_f16 normal) {
    // Collider 2 is static(move a oiut of b)
    // Move A along the normal by the penetration amount.
    // This resolves the collision by pushing A out of B.
    if (g_colliders[bId].isStatic) {

        g_positions[aId].x -= F16_mul(normal.x, penetration);
        g_positions[aId].y -= F16_mul(normal.y, penetration);

    } else if(g_colliders[aId].isStatic) {                   

        g_positions[bId].x -= F16_mul(FIX16(0) - normal.x, penetration);
        g_positions[bId].y -= F16_mul(FIX16(0) - normal.y, penetration);

    } else {

        // Move both objects by half the penetration in opposite directions.                    
        fix16 half_pen = F16_div(penetration, FIX16(2));
        g_positions[aId].x  -= F16_mul(normal.x, half_pen);
        g_positions[aId].y -= F16_mul(normal.y, half_pen);
        g_positions[bId].x  += F16_mul(normal.x, half_pen);
        g_positions[bId].y  += F16_mul(normal.y, half_pen);
    }
}

// Sweep and prune
void CollisionSystem_update() {
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
    //TODO: Figure out how qsort works in SGDK and use it instead
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

            // Use the second entityId from the ordered list to get the doby for testing
            EntityId collider2Id = g_active_colliders[j];
            ColliderComponent collider2 = g_colliders[ collider2Id ];

            // Do not attempt to check collisions when both are static bodies.
            if (collider1.isStatic && collider2.isStatic) {
                //LOGGER_DEBUG("Collision: Skipping static bodies: BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
                break;
            }

            // Check if the colliders are in the same layer, and has the same mask
            if ( ((collider1.layer & collider2.iteractWith) == 0) || ((collider2.layer & collider1.iteractWith) == 0)  ) {
                //LOGGER_DEBUG("Collision: Skipping DIFFERENT layers: BODYIES %d, %d", collider1Id, collider2Id);
                break;
            }

            // Pruning step:
            // If body2's left edge is beyond body1's right edge,
            // then no subsequent bodies (which are sorted further to the right)
            // can overlap with body1 on the X-axis.
            //if (body2->shape.colliderShape.box.min.x > body1->shape.colliderShape.box.max.x) {            
            if ( F16_toInt(g_positions[collider2Id].x) >
                 F16_toInt(g_positions[collider1Id].x) + 
                 (s16)(collider1.shape.box.x + collider1.shape.box.w)
            ) {
                // LOGGER_DEBUG("Collision: Prunning BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
                break; // Prune this inner loop for body1->
            }           

            //LOGGER_DEBUG("Collision: BODIES %d,%d - LAYERS %d,%d - MASKS: %d,%d", collider1Id, collider2Id, collider1.layer, collider2.layer, collider1.iteractWith, collider2.iteractWith);

            if(CollisionSystem_checkForCollision(collider1Id, collider2Id, &penetration, &normal) ) {
                // LOGGER_DEBUG("COLLIDED %d, %d, pen: %d normal: %d, %d, static, %d, %d", collider1Id, collider2Id, penetration, normal.x, normal.y, g_colliders[collider1Id].isStatic, g_colliders[collider2Id].isStatic);
                
                // The collision resolution will not happen over triggers.
                // only the event will happen
                if(!collider1.isTrigger && !collider2.isTrigger) {

                    CollisionSystem_resolveCollision (
                        collider1Id,
                        collider2Id,
                        penetration,
                        normal
                    );

                }

                // Raise the collisino event
                Event collisionEvent = {
                    .type = EVT_COLLISION,
                    .data.collision = {
                        .entityA = collider1Id,
                        .entityB = collider2Id,
                        .normal = normal,
                        .penetration = penetration
                    }
                };
                
                Event_publish(&collisionEvent);                
            }
        }
    }
}

void RenderSystem_update() {
    // This system needs Position (to know where) and Sprite (to know what)
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE;

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if (Entity_hasAllComponents(i, required_mask)) {
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

