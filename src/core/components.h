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

//Layer: Represents the type of object. Each object belongs to exactly one layer. Layers are defined as powers of two (e.g., 1, 2, 4, 8...).
typedef enum {
    LAYER_NONE               = 0,        // Represents no components (all bits off)
    LAYER_1                  = 1 << 0,
    LAYER_2                  = 1 << 1,
    LAYER_3                  = 1 << 2,
    LAYER_4                  = 1 << 3,
    LAYER_5                  = 1 << 4,
    LAYER_6                  = 1 << 5,
    LAYER_7                  = 1 << 6,
    LAYER_8                  = 1 << 7,
    LAYER_9                  = 1 << 8, 
    MAX_COLLIDER_LAYER       = 1 << 9
} ColliderLayer;

typedef u32 ColliderLayerMask;

typedef struct {    
    union {
       AABBColliderType box; 
       CircleColliderType circle;
    } shape;
    ColliderType type;
    ColliderLayer layer;            // Collision layer
    ColliderLayerMask iteractWith;  // Other collision layers it iteract with eg.: LAYER_1 && LAYER_2
    bool isStatic;
    bool isTrigger;
} ColliderComponent;

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