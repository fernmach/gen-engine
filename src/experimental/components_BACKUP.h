#ifndef _ENG_COMPONENTS_H_
#define _ENG_COMPONENTS_H_

#include "config.h"

// SGDK forward declaration
#include <genesis.h>

// Forward declarion for entity id definition
#if ECS_MAX_ENTITIES > 255
    typedef u16 EntityId;
#else
    typedef u8 EntityId;
#endif

// This typedef will store the COMBINED flags for an entity
// Use u16 if you have <= 16 components, u32 for <= 32
// SGDK often uses u16 for efficiency if possible.
#if (ECS_MAX_COMPONENTS <= 16)
    typedef u16 ComponentMask;
#elif (ECS_MAX_COMPONENTS <= 32)
    typedef u32 ComponentMask;
#endif

// List of component types supported by the engin
// Uses bit masking
typedef enum {
    COMPONENT_NONE                      = 0,        // Represents no components (all bits off)
    COMPONENT_POSITION                  = 1 << 0,   // Bit 0 (Value: 1)
    COMPONENT_VELOCITY                  = 1 << 1,   // Bit 1 (Value: 2)
    COMPONENT_COLLIDER                  = 1 << 2,   // Bit 2 (Value: 2)
    COMPONENT_SPRITE                    = 1 << 3,   // Bit 3 (Value: 4)
    COMPONENT_SCREEN_CONSTRAINT         = 1 << 4,   // Bit 4 (Value: 4)

    COMPONENT_MAX_VALUE                 = 1 << 5
} ComponentType;

// --- Component Types ---
// Position component
// TODO: Using fixed point numbers for position and velocity, 
// Sprite definition uses s16 for x/y evaluate if better then fix16
typedef struct {
    fix16 x; 
    fix16 y; 
} PositionComponent;

// Velocity component
// TODO: Using fixed point numbers for position and velocity, 
// Sprite definition uses s16 for x/y evaluate if better then fix16
typedef struct {
    fix16 dx;
    fix16 dy;
} VelocityComponent;

// Collider component
typedef struct {
    s8 x;
    s8 y; 
    u8 w; 
    u8 h;
} AABBColliderType;

typedef struct {
    s8 x;
    s8 y;
    u16 radius;
} CircleColliderType;

typedef enum {
    COLLIDER_TYPE_AABB   = 1,
    COLLIDER_TYPE_CIRCLE = 2
} ColliderType;

typedef struct {
    union {
       AABBColliderType box; 
       CircleColliderType circle;
    } shape;
    ColliderType type;
    bool isStatic;
} ColliderComponent;

// Density and restitution examples.
// Rock       Density : 0.6  Restitution : 0.1
// Wood       Density : 0.3  Restitution : 0.2
// Metal      Density : 1.2  Restitution : 0.05
// BouncyBall Density : 0.3  Restitution : 0.8
// SuperBall  Density : 0.3  Restitution : 0.95
// Pillow     Density : 0.1  Restitution : 0.2
// Static     Density : 0.0  Restitution : 0.4
//https://www.engineeringtoolbox.com/density-solids-d_1265.html
//Min density = 0.5
//Max density = 21.4
typedef struct
{
    fix16 density;
    fix16 mass;
    //fix16 inverseMass;
    fix16 restitution;
    fix16 area;
    bool isStatic;
    u8 width; 
    u8 heith;
} RigidBody;


// Rigid body component
//TODO: Implement more robust physics. For now only AABB vs AABB avaialble.
//TODO: Move the transform, velocity and acceleration into the rigid body definition
//typedef Collider RigidBodyComponent;

// Screen constrating component
typedef struct {
    bool x;
    bool y;
} ScreenConstraintComponent;

// Sprite component
typedef struct {
    Sprite* sgdkSprite;         // Pointer to the SGDK sprite object
                            // Added For collision or other logic    
                            // Add animation data, palette, etc. here if needed
} SpriteComponent;
//typedef Sprite* SpriteComponent;

// --- Global Component Arrays ---
// These arrays store the actual component data, indexed by EntityID
extern PositionComponent            g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent            g_velocities[ECS_MAX_ENTITIES];
extern ColliderComponent            g_colliders[ECS_MAX_ENTITIES];
extern SpriteComponent              g_sprites[ECS_MAX_ENTITIES];
extern ScreenConstraintComponent    g_screen[ECS_MAX_ENTITIES];

#endif // _ENG_COMPONENTS_H_