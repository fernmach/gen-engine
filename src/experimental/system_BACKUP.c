/*
// #include "logger.h"
// #include "asserts.h"
// #include "memory.h"
// #include "event.h"
// #include "input.h"
// #include "components.h"
// #include "ecs.h"
// #include "utils.h"
// #include "vector_math.h"

// #include <genesis.h>

// // --- Global component array definitions ---
// extern PositionComponent    g_positions[ECS_MAX_ENTITIES];
// extern VelocityComponent    g_velocities[ECS_MAX_ENTITIES];
// extern ColliderComponent    g_colliders[ECS_MAX_ENTITIES];
// extern SpriteComponent      g_sprites[ECS_MAX_ENTITIES];

// // --- Global entity tracking array definitions  ---
// extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// // Temporary array to hold pointers to *active* physics components for sorting.
// // This is crucial to avoid sorting inactive components or copying large structs.
// // Used for sweep and prune broad phase colligion detction
// static u16 g_active_colliders_count = 0;

// //Should be EntityId
// static u16 g_active_colliders[ECS_MAX_ENTITIES];

// void MovementSystem_update(fix16 dt) {
//     // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

//     for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
//         //RenderSystemDebug_clearText(i);
//         if (Entity_hasAllComponents(i, required_mask)) {
//             g_positions[i].x += F16_mul(g_velocities[i].dx, dt);
//             g_positions[i].y += F16_mul(g_velocities[i].dy, dt);            
//             //LOGGER_DEBUG("Running %d %d", g_positions[i].x, g_positions[i].y);
//         }
//     }
// }

// void ScreenConstraintSystem_update() {
//      // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE |COMPONENT_SCREEN_CONSTRAINT;

//     const u16 screen_width = VDP_getScreenWidth();
//     const u16 screen_height = VDP_getScreenHeight();
//     //const u16 screen_width = 320;
//     //const u16 screen_height = 224;

//     //LOGGER_DEBUG("%d, %d", screen_width, screen_height);

//     for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
//         if (Entity_hasAllComponents(i, required_mask)) {

//             // ... (screen bounds logic) ...
//             //s16 screen_x = F16_toInt(g_positions[i].x);
//             //s16 screen_y = F16_toInt(g_positions[i].y);
            
//             //screen x
//             if (F16_toInt(g_positions[i].x) < 0) {                
//                 g_positions[i].x = FIX16(0);
//                 g_velocities[i].dx = -g_velocities[i].dx;                
//             }
            
//             //screen y
//             if (F16_toInt(g_positions[i].y) < 0) {                
//                 g_positions[i].y = FIX16(0);
//                 g_velocities[i].dy = -g_velocities[i].dy;
//             }

//             if ((F16_toInt(g_positions[i].x) + g_sprites[i].sgdkSprite->definition->w) > screen_width) {                    
//                 g_positions[i].x = FIX16(screen_width - g_sprites[i].sgdkSprite->definition->w);                    
//                 g_velocities[i].dx = -g_velocities[i].dx;                    
//             }

//             if ((F16_toInt(g_positions[i].y) + g_sprites[i].sgdkSprite->definition->h) > screen_height) {
//                 g_positions[i].y = FIX16(screen_height - g_sprites[i].sgdkSprite->definition->h);
//                 g_velocities[i].dy = -g_velocities[i].dy;
//             }
//         }
//     }
// }

// bool CollisionSystem_AABBvsAABB(EntityId aId, EntityId bId ) {
//     // Exit with no intersection if found separated along an axis
//     PositionComponent posA = g_positions[aId];
//     ColliderComponent collA = g_colliders[aId];

//     PositionComponent posB = g_positions[bId];
//     ColliderComponent collB = g_colliders[bId];

//     fix16 a_min_left = posA.x;
//     fix16 a_min_right = posA.y;
//     fix16 a_max_left = posA.x + FIX16(collA.shape.box.x + collA.shape.box.w);
//     fix16 a_max_right = posA.y + FIX16(collA.shape.box.y + collA.shape.box.h);

//     fix16 b_min_left = posB.x;
//     fix16 b_min_right = posB.y;
//     fix16 b_max_left = posB.x + FIX16(collB.shape.box.x + collB.shape.box.w);
//     fix16 b_max_right = posB.y + FIX16(collB.shape.box.y + collB.shape.box.h);  

//     //if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
//     if(a_max_left < b_min_left || a_min_left > b_max_left) return false;
//     if(a_max_right < b_min_right || a_min_right > b_max_right) return false;
//     // No separating axis found, therefor there is at least one overlapping axis 
//     return true;    
// }

// // // collider and positions for check collisions
// // bool CollisionSystem_AABBvsAABB(
// //     ColliderComponent aCol, PositionComponent aPos,
// //     ColliderComponent bCol, PositionComponent bPos
// // ) {
// //     fix16 a_min_left = aPos.x;
// //     fix16 a_min_right = aPos.y;
// //     fix16 a_max_left = aPos.x + FIX16(aCol.shape.box.x + aCol.shape.box.w);
// //     fix16 a_max_right = aPos.y + FIX16(aCol.shape.box.y + aCol.shape.box.h);

// //     fix16 b_min_left = bPos.x;
// //     fix16 b_min_right = bPos.y;
// //     fix16 b_max_left = bPos.x + FIX16(bCol.shape.box.x + bCol.shape.box.w);
// //     fix16 b_max_right = bPos.y + FIX16(bCol.shape.box.y + bCol.shape.box.h);  

// //     //if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
// //     if(a_max_left < b_min_left || a_min_left > b_max_left) return false;
// //     if(a_max_right < b_min_right || a_min_right > b_max_right) return false;
// //     // No separating axis found, therefor there is at least one overlapping axis 
// //     return true;    
// // }

// bool CollisionSystem_compareByLeftEdge(u16 i, u16 j) {
//     return g_positions[i].x > g_positions[j].x;    
// }

// //Circle collision resolution
// // TODO: Not finished
// void CollisionSystem_resolveCollision( EntityId aId, EntityId bId, Vect2D_f16 normal ) {

//     // Calculate relative velocity 
//     //Vec2 rv = B.velocity - A.velocity;
//     Vect2D_f16 velocityA = V2D_f16(g_velocities[aId].dx, g_velocities[aId].dy);
//     Vect2D_f16 velocityB = V2D_f16(g_velocities[bId].dx, g_velocities[bId].dy);

//     Vect2D_f16 relativeVelocity = V2D_f16_sub( velocityB, velocityA);
    
//     // To obtain the norma we need center of a and b
//     // TODO: Add the relative position fo the collider from the position of the object
//     // Vect2D_f16 centerA = V2D_f16 (
//     //     F16_div(g_positions[aId].x + g_colliders[aId].shape.box.x + g_colliders[aId].shape.box.w, 2),
//     //     F16_div(g_positions[aId].y + g_colliders[aId].shape.box.y + g_colliders[aId].shape.box.h, 2),
//     // );

//     // Vect2D_f16 centerB = V2D_f16(
//     //     F16_div(g_positions[bId].x + g_colliders[bId].shape.box.x + g_colliders[bId].shape.box.w, 2),
//     //     F16_div(g_positions[bId].y + g_colliders[bId].shape.box.y + g_colliders[bId].shape.box.h, 2),
//     // );

//     // // Obtain the collision normal
//     // Vect2D_f16 normal = V2D_f16_normalize( V2D_f16_sub(centerB, centerA) );

//     // Calculate relative velocity in terms of the normal direction 
//     fix16 velAlongNormal = V2D_f16_dot( relativeVelocity, normal );

//     // Do not resolve if velocities are separating 
//     if(velAlongNormal > FIX16(0))
//         return;

//     // 
//     // Calculate restitution 
//     //f16 e = min( A.restitution, B.restitution );
    
//     // Fixed testing (bouny ball)
//     fix16 e = FIX16(0.8);

//     fix16 aDensity = FIX16(0.3);
//     fix16 aArea = FIX16(g_colliders[aId].shape.box.w * g_colliders[aId].shape.box.h);
//     fix16 aMass = F16_mul(aArea, aDensity);
//     fix16 aInvMass = F16_div(FIX16(1.0), aMass);

//     fix16 bDensity = 0.3;
//     fix16 bArea = FIX16(g_colliders[bId].shape.box.w * g_colliders[bId].shape.box.h);
//     fix16 bMass = F16_mul(bArea, bDensity);
//     fix16 bInvMass = F16_div(FIX16(1.0), bMass);
    
//     // float j = -(1 + e) * velAlongNormal
//     fix16 j = F16_mul(FIX16(0) - (FIX16(1) + e), velAlongNormal);
//     // j /= 1 / A.mass + 1 / B.mass
//     j = F16_div(j, F16_div(FIX16(1), aMass) + F16_div(FIX16(1), bMass) );

//     // Calculate impulse scalar
//     // Calculate impulse scalar 
//     // float j = -(1 + e) * velAlongNormal
//     // j /= 1 / A.mass + 1 / B.mass
//     // fix16 j = F16_mul(0 - (FIX16(1) + e), velAlongNormal);
//     //j /= FIX16(1) / A.mass + FIX16(1) / B.mass;
    

//     // Calculate impulse
//     Vect2D_f16 impulse = V2D_f16_mul_scalar(normal, j);    

//     // Apply impulse
//     //A.velocity -= 1 / A.mass * impulse;    
//     //B.velocity += 1 / B.mass * impulse;
//     Vect2D_f16 deltaVelocityA = V2D_f16_mul_scalar(impulse, aInvMass);
//     Vect2D_f16 newVelocityA = V2D_f16_sub(velocityA, deltaVelocityA);
//     g_velocities[aId].dx = newVelocityA.x;
//     g_velocities[aId].dy = newVelocityA.y;

//     Vect2D_f16 deltaVelocityB = V2D_f16_mul_scalar(impulse, bInvMass);
//     Vect2D_f16 newVelocityB  = V2D_f16_add(velocityB, deltaVelocityB);    
//     g_velocities[bId].dx = newVelocityB.x;
//     g_velocities[bId].dy = newVelocityB.y;    
// }

// //https://code.tutsplus.com/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331t
// bool CollisionSystem_checkForCollision( EntityId aId, EntityId bId, 
//     fix16* out_penetration, Vect2D_f16* out_normal )
// {
//     // //fix16 penetration = FIX16(0);
//     // //Vect2D_f16 normal = V2D_f16( FIX16(0), FIX16(0) );

//     // // Vector from A to B
//     // // Vec2 n = B->pos - A->pos
//     // Vect2D_f16 n = V2D_f16_sub( 
//     //     V2D_f16(g_positions[bId].x, g_positions[bId].y),
//     //     V2D_f16(g_positions[aId].x, g_positions[aId].y)
//     // );

//     // LOGGER_DEBUG("Normal {%d,%d}", n.x, n.y);

//     // // Calculate half extents along x axis for each object 
//     // //float a_extent = (abox.max.x - abox.min.x) / 2
//     // //float b_extent = (bbox.max.x - bbox.min.x) / 2
//     // f16 a_extent = F16_div(
//     //     ( g_positions[aId].x + FIX16( g_colliders[aId].shape.box.x + g_colliders[aId].shape.box.w) ) - g_positions[aId].x,
//     //     2
//     // );

//     // f16 b_extent = F16_div(
//     //     ( g_positions[bId].x + FIX16(g_colliders[bId].shape.box.x + g_colliders[bId].shape.box.w)) - g_positions[bId].x,
//     //     2
//     // );

//     // LOGGER_DEBUG("Extent a/b {%d , %d}", a_extent, b_extent);

//     // // Calculate overlap on x axis
//     // f16 x_overlap = a_extent + b_extent - abs( n.x );

//     // LOGGER_DEBUG("X overlap %d}", x_overlap);

//     // // SAT test on x axis
//     // if(x_overlap > 0)
//     // {
//     //     // Calculate half extents along x axis for each object 
//     //     // float a_extent = (abox.max.y - abox.min.y) / 2
//     //     // float b_extent = (bbox.max.y - bbox.min.y) / 2
//     //     f16 a_extent = F16_div(
//     //         ( g_positions[aId].y + FIX16( g_colliders[aId].shape.box.y + g_colliders[aId].shape.box.h) ) - g_positions[aId].y,
//     //         2
//     //     );

//     //     f16 b_extent = F16_div(
//     //         ( g_positions[bId].y + FIX16(g_colliders[bId].shape.box.y + g_colliders[bId].shape.box.h)) - g_positions[bId].y,
//     //         2
//     //     );

//     //     // Calculate overlap on x axis
//     //     f16 y_overlap = a_extent + b_extent - abs( n.y );

//     //      // SAT test on y axis 
//     //     if(y_overlap > 0)
//     //     {
//     //          // Find out which axis is axis of least penetration 
//     //         if(x_overlap > y_overlap)
//     //         {
//     //             // Point towards B knowing that n points from A to B 
//     //             if(n.x < 0) {
//     //                 *normal = V2D_f16( FIX16(-1), FIX16(0) );
//     //             } else {
//     //                 *normal = V2D_f16( FIX16(0), FIX16(0) );
//     //             }
//     //             *penetration = x_overlap;
//     //             return TRUE;
//     //         } else {            
//     //             // Point towards B knowing that n points from A to B 
//     //             if(n.y < 0) {
//     //                 *normal = V2D_f16( FIX16(0), FIX16(-1) );
//     //             } else {
//     //                 *normal = V2D_f16( FIX16(0), FIX16(1) );
//     //             }
//     //             *penetration = y_overlap;
//     //             return TRUE;
//     //         }            
//     //     }
//     // }
//     // return FALSE;

//      // 1. Get components from the ECS
//     PositionComponent posA = g_positions[aId];
//     ColliderComponent collA = g_colliders[aId];

//     PositionComponent posB = g_positions[bId];
//     ColliderComponent collB = g_colliders[bId];

//     // Note: Correctly calculating the AABB bounds. 
//     // It's assumed pos is the top-left of the entity, and the collider's x/y is an offset.
//     fix16 a_min_x = posA.x + FIX16(collA.shape.box.x);
//     fix16 a_min_y = posA.y + FIX16(collA.shape.box.y);
//     fix16 a_w = FIX16(collA.shape.box.w);
//     fix16 a_h = FIX16(collA.shape.box.h);

//     fix16 b_min_x = posB.x + FIX16(collB.shape.box.x);
//     fix16 b_min_y = posB.y + FIX16(collB.shape.box.y);
//     fix16 b_w = FIX16(collB.shape.box.w);
//     fix16 b_h = FIX16(collB.shape.box.h);
    
//     // 2. Calculate center points and half-extents (half-widths/heights)
//     fix16 a_half_w = F16_div(a_w, FIX16(2));
//     fix16 a_half_h = F16_div(a_h, FIX16(2));
//     Vect2D_f16 centerA = { a_min_x + a_half_w, a_min_y + a_half_h };

//     fix16 b_half_w = F16_div(b_w, FIX16(2));
//     fix16 b_half_h = F16_div(b_h, FIX16(2));
//     Vect2D_f16 centerB = { b_min_x + b_half_w, b_min_y + b_half_h };

//     // 3. Calculate the vector from the center of A to the center of B
//     Vect2D_f16 n = { centerB.x - centerA.x, centerB.y - centerA.y };

//     // 4. Calculate the extent of overlap on each axis
//     fix16 x_overlap = (a_half_w + b_half_w) - F16_abs(n.x);
//     fix16 y_overlap = (a_half_h + b_half_h) - F16_abs(n.y);

//     // 5. If there is no overlap on either axis, there is no collision
//     if (x_overlap <= 0 || y_overlap <= 0) {
//         return false;
//     }

//     // 6. Find the axis of minimum penetration
//     if (x_overlap < y_overlap) {
//         // Pointing right if n.x > 0, left if n.x < 0
//         *out_penetration = x_overlap;
//         out_normal->x = (n.x < 0) ? FIX16(-1) : FIX16(1);
//         out_normal->y = FIX16(0);
//     } else {
//         // Pointing down if n.y > 0, up if n.y < 0
//         *out_penetration = y_overlap;
//         out_normal->x = FIX16(0);
//         out_normal->y = (n.y < 0) ? FIX16(-1) : FIX16(1);
//     }

//     return true;
// }

// // Sweep and prune
// void CollisionSystem_update() {
//     // Define the components this system operates on
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_COLLIDER;

//     fix16 penetration;
//     Vect2D_f16 normal;

//     // Currently active rigid bodies
//     g_active_colliders_count = 0;

//     // Create the array that will be sorted
//     for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
//         if (Entity_hasAllComponents(i, required_mask)) {
//             g_active_colliders[g_active_colliders_count++] = i;            
//             //LOGGER_DEBUG("Collision: Active rigid body pos: %d, id: %d", g_active_colliders_count, i);
//         }
//     }

//     // Not enough bodies to check for collisions
//     if (g_active_colliders_count < 2) {
//         //LOGGER_DEBUG("Collision: Not enough bodies to check for collisions");
//         return;
//     }

//     //Order
//     //TODO: Figure out how qsort works in SGDK and use it instead
//     insertionSort(g_active_colliders,
//         g_active_colliders_count,
//         CollisionSystem_compareByLeftEdge);
//     //LOGGER_DEBUG('ORDERED BODIES');
//     //printArray(g_active_rb, g_active_rb_count);

//     //Sweep
//     for (EntityId i = 0; i < g_active_colliders_count; ++i) {

//         // Use the first entityId from the ordered list to get the doby for testing
//         EntityId collider1Id = g_active_colliders[i];
//         ColliderComponent collider1 = g_colliders[ collider1Id ];
        
//         // Check colligion agains all entities and break if prune
//         for (EntityId j = i+1; j < g_active_colliders_count; ++j) {
//             EntityId collider2Id = g_active_colliders[j];            
            
//             // Pruning step:
//             // If body2's left edge is beyond body1's right edge,
//             // then no subsequent bodies (which are sorted further to the right)
//             // can overlap with body1 on the X-axis.
//             //if (body2->shape.colliderShape.box.min.x > body1->shape.colliderShape.box.max.x) {            
//             if ( F16_toInt(g_positions[collider2Id].x) >
//                  F16_toInt(g_positions[collider1Id].x) + 
//                  (s16)(collider1.shape.box.x + collider1.shape.box.w)
//             ) {
//                 //LOGGER_DEBUG("Collision: Prunning BODY 1 - %d, BODY 2 - %d", collider1Id, collider2Id);
//                 break; // Prune this inner loop for body1->
//             }           

//             if(CollisionSystem_checkForCollision(collider1Id, collider2Id, &penetration, &normal) ) {
//                 LOGGER_DEBUG("Collision: Dectected colligion beween %d && %d", collider1Id, collider2Id);
//                 LOGGER_DEBUG("    Collision details  pen: %d, normal {%d, %d})", penetration, normal.x, normal.y);
//                 //TODO: Raise collision event with
//                 //TODO: Resolve collision(static and diynamic)                

//                 // Collision detected!
//                 // The normal points from A to B. We can use it to move B out of A. 
//                 //PositionComponent* posB = &g_positions[entityB]; // Get a pointer to modify it
//                 // Move B along the normal by the penetration amount.
//                 // This resolves the collision by pushing B out of A.
//                 //g_positions[collider2Id].x += F16_mul(normal.x, penetration);
//                 //g_positions[collider2Id].y += F16_mul(normal.y, penetration);
//                 // You could also move both objects by half the penetration in opposite directions.
//                 // PositionComponent* posA = &g_positions[entityA];
//                 fix16 half_pen = F16_div(penetration, FIX16(2));
//                 g_positions[collider1Id].x  -= F16_mul(normal.x, half_pen);
//                 g_positions[collider1Id].y -= F16_mul(normal.y, half_pen);
//                 g_positions[collider2Id].x  += F16_mul(normal.x, half_pen);
//                 g_positions[collider2Id].y  += F16_mul(normal.y, half_pen);

//                 //CollisionSystem_resolveCollision( collider1Id, collider2Id, normal);
//             }
//         }
//     }
// }

// //Brute force (working properly);
// // void CollisionSystem_update() {
// //     // Define the components this system operates on
// //     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_RIGID_BODY;

// //     for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
// //         if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {

// //             // Need to take into consideration the sprite position on the screen
// //             // AABBColliderShape a = {
// //             //     {
// //             //         F16_toInt(g_positions[i].x),
// //             //         F16_toInt(g_positions[i].y)
// //             //     },
// //             //     {
// //             //         F16_toInt(g_positions[i].x) + g_rigid_bodies[i].shape.colliderShape.box.max.x,
// //             //         F16_toInt(g_positions[i].y) + g_rigid_bodies[i].shape.colliderShape.box.max.y
// //             //     }
// //             // };            
            
// //             AABBColliderShape a;
// //             a.min.x = F16_toInt(g_positions[i].x);
// //             a.min.y = F16_toInt(g_positions[i].y);
// //             a.max.x = F16_toInt(g_positions[i].x) + g_rigid_bodies[i].collider.shape.box.max.x;
// //             a.max.y = F16_toInt(g_positions[i].y) + g_rigid_bodies[i].collider.shape.box.max.y;
            
// //             // Check colligion agains all entityes 
// //             for (EntityId j = i+1; j < ECS_MAX_ENTITIES; ++j) {
                
// //                 //if (i == j)
// //                   //  continue;

// //                 if (g_entity_active[j] && Entity_hasAllComponents(j, required_mask)) {

// //                     // // Function to check for collision between two entities (Axis-Aligned Bounding Box)
// //                     // bool checkCollision(const Entity* entity1, const Entity* entity2) {
// //                     //     return (entity1->position.x < entity2->position.x + entity2->size.width &&
// //                     //             entity1->position.x + entity1->size.width > entity2->position.x &&
// //                     //             entity1->position.y < entity2->position.y + entity2->size.height &&
// //                     //             entity1->position.y + entity1->size.height > entity2->position.y);
// //                     // }
                    
// //                     AABBColliderShape b;
// //                     b.min.x = F16_toInt(g_positions[j].x);
// //                     b.min.y = F16_toInt(g_positions[j].y);
// //                     b.max.x = F16_toInt(g_positions[j].x) + g_rigid_bodies[j].collider.shape.box.max.x;
// //                     b.max.y = F16_toInt(g_positions[j].y) + g_rigid_bodies[j].collider.shape.box.max.x;

// //                     // LOGGER_DEBUG("Sprite width: A %d, %d", 
// //                     //     F16_toInt(g_sprites[i].sgdkSprite->definition->w),
// //                     //     F16_toInt(g_sprites[i].sgdkSprite->definition->h)
// //                     // );

// //                     // LOGGER_DEBUG("Position: A %d, %d -- B %d, %d", 
// //                     //     F16_toInt(g_positions[i].x), F16_toInt(g_positions[i].y),
// //                     //     F16_toInt(g_positions[j].x), F16_toInt(g_positions[j].y));

// //                     // LOGGER_DEBUG("A -- Min %d, %d -- Max %d, %d", 
// //                     //     a.min.x, a.min.y,
// //                     //     a.max.x, a.max.y);

// //                     // LOGGER_DEBUG("B -- Min %d, %d -- Max %d, %d", 
// //                     //     b.min.x, b.min.y,
// //                     //     b.max.x, b.max.y);

// //                     //ASSERT_EXP(1==0);

// //                     if ( AABBvsAABB( a, b ) ) {
// //                         //LOGGER_DEBUG("Collision detected: Entities %d, %d", i, j);
// //                     }
// //                 }
// //             }
// //         }
// //     }
// // }

// void RenderSystem_update() {
//     // This system needs Position (to know where) and Sprite (to know what)
//     const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE;

//     for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
//         if (Entity_hasAllComponents(i, required_mask)) {
//             if (g_sprites[i].sgdkSprite != NULL) { // Still good to check the actual sprite pointer

//                 //LOGGER_DEBUG("Entity %d position {%d, %d}", i, g_positions[i].x, g_positions[i].y);
//                 //LOGGER_DEBUG("Entity %d screen {%d, %d}", i, screen_x, screen_y);

//                 // Using F16_toInt would be the correct way to go but the numbers
//                 // are beeing incorrectly retured.
//                 s16 screen_x = F16_toInt(g_positions[i].x);
//                 s16 screen_y = F16_toInt(g_positions[i].y);

//                 SPR_setPosition(g_sprites[i].sgdkSprite, screen_x, screen_y);        
//                 // ... other sprite updates ...

//                 // Update animation, visibility, etc. based on SpriteComponent data
//                 // SPR_setAnim(g_sprites[i].sgdk_sprite, g_sprites[i].current_anim_frame);
//                 // SPR_setVisibility(g_sprites[i].sgdk_sprite, VISIBLE / HIDDEN);
//             }
//         }
//     }
// }

*/