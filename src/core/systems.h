#ifndef _ENG_SYSTEMS_H_
#define _ENG_SYSTEMS_H_

// Forward declaration for SGDK types
typedef short s16;
typedef s16 fix16;

// --------- Movement System ------------
void MovementSystem_update(fix16 dt);

// ---------Colligion resolution/detection ----------------
// Collision resolution
void CollisionSystem_AABBvsAABB( EntityId aId, EntityId bId );
// Broad phase dection
bool CollisionSystem_compareByLeftEdge(u16 a, u16 b);
// Narrow phase detection
void CollisionSystem_update();

// --------- Screen constraint system ----------------
void ScreenConstraintSystem_update();

// ---------Render System ----------------
void RenderSystem_update();

#endif // SYSTEMS_H
