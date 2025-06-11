#ifndef _ENG_PHYSICS_H_
#define _ENG_PHYSICS_H_

#include <genesis.h>

typedef struct
{
  Vect2D_s16 min; // top left corner
  Vect2D_s16 max; // bottom right corner.
} AABBColliderShape;

// Circle
typedef struct
{
  fix16 radius;
  Vect2D_s16 position;
} CircleColliderShape; //CircleColliderShapeType

typedef enum {
    SHAPE_TYPE_AABB   = 1,
    SHAPE_TYPE_CIRCLE = 2
} ColliderType;

typedef struct {
    union {
        AABBColliderShape box;
        CircleColliderShape circle;
    } shape;
    ColliderType type;
} Collider;

// typedef struct {
//   Position position
//   Velocity velocity
//   Acceleration acceleration
// } Transform;

typedef struct {
  // Position position
  // Velocity velocity
  // Acceleration acceleration
  Collider collider;
} RigidBody;

// public api implementation
// Collision detection using Separating Axis Theorem (SAT).
bool AABBvsAABB( AABBColliderShape a, AABBColliderShape b );

// Collision detection using two radious cirle distances
bool CirclevsCircle( CircleColliderShape a, CircleColliderShape b );

// Collision resolution
//void ResolveCollision( Body A, Body B );

#endif //_ENG_PHYSICS_H_