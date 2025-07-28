#ifndef _ENG_COMPONENTS_H_
#define _ENG_COMPONENTS_H_

#include "config.h"
#include "defines.h"
#include "fsm.h"

// SGDK forward declaration
#include <genesis.h>

// Forward declare the FSM definition
struct FSMDefinition;

// This typedef will store the COMBINED flags for an entity
// Use u16 if you have <= 16 components, u32 for <= 32
// SGDK often uses u16 for efficiency if possible.
#if (ECS_MAX_COMPONENTS <= 16)
    typedef u16 ComponentMask;
    #define DECLARE_COMPONENT_MASK typedef u16 ComponentMask
#elif (ECS_MAX_COMPONENTS <= 32)
    typedef u32 ComponentMask;
    #define DECLARE_COMPONENT_MASK typedef u32 ComponentMask
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
    COMPONENT_FSM                       = 1 << 5,   // Bit 4 (Value: 4)

    COMPONENT_MAX_VALUE                 = 1 << 6
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

// TODO: Create an EdgeColliderComponent to trigger behavior when
// and entity hits one or more edges of the screen (requires Camera 
// implementation)
/*
typedef enum {
    EDGE_NONE           = 0,        
    EDGE_LEFT           = 1 << 0,
    EDGE_RIGHT          = 1 << 1,
    EDGE_TOP            = 1 << 2,
    EDGE_BOTTOM         = 1 << 3,    
    EDGE_TYPE_COUNT     = 5
} Edge;

typedef enum {
    EDGE_RESOLUTION_NONE,
    EDGE_RESOLUTION_STOP,
    EDGE_RESOLUTION_STOP_AND_TRIGGER,
    EDGE_RESOLUTION_INVERT_VELOCITY,
    EDGE_RESOLUTION_INVERT_VELOCITY_AND_TRIGGER,
    EDGE_RESOLUTION_TRIGGER,
    EDGE_RESOLUTION_COUNT
} EdgeColliderBehavior;

typedef u16 EdgeTypeMask;

// Will trigger a behavior when a collisin happens in the 
// define screen boundaries.
typedef struct {
    EdgeTypeMask edgeMask;
    EdgeColliderBehavior edgeColliderBehavior;
} EdgeColliderComponent;
*/

// typedef enum {
//     SPR_PROPETY_NONE            = 0,        // Represents no components (all bits off)
//     SPR_PROPETY_ANIM            = 1 << 0,   // Bit 0 (Value: 1)
//     SPR_PROPETY_DEPTH           = 1 << 1,   // Bit 1 (Value: 2)
//     SPR_PROPETY_VISIBILITY      = 1 << 2,   // Bit 2 (Value: 2)
//     SPR_PROPETY_VFLIP           = 1 << 3,   // Bit 3 (Value: 4)
//     SPR_PROPETY_HFLIP           = 1 << 4,   // Bit 4 (Value: 4)    

//     SPR_PROPETY_COUNT           = 1 << 5
// } SpritePropertyType;

// typedef u16 SpritePropertyMask;

// typedef struct {    
//     s16 animationId;        // Animation to be played
//     s16 depth;              // current sprite depth (Z) position used for Z sorting
//     u16 visibility;         // visibility information of current frame for each VDP sprite (max = 16)
//     bool vFlip;             // Set sprite Vertical Flip attribut.
//     bool hFlip;             // Set sprite Horizontal Flip attribut.
// } SpriteComponentProperties;

// Sprite component
typedef struct {
    Sprite* sgdkSprite;         // Pointer to the SGDK sprite object
                                // Added For collision or other logic    
                                // Add animation data, palette, etc. here if needed
    //SpriteComponentProperties properties;
    //SpritePropertyMask propertiesToUpdate;
    bool hFlip;
    bool isHFlipDirty;
} SpriteComponent;
//typedef Sprite* SpriteComponent;

// The FSM Component. This is the part that gets attached to an entity.
// It's lightweight, containing only the current state and a timer.
typedef struct FSMComponent {
    u16 currentState;            // The current state, as an index into the FSMDefinition's states array
    s16 stateTimer;              // A generic timer for states to use (e.g., "stay in attack animation for 30 frames")
    const FSMDefinition* definition; // Pointer to the entity's behavior definition
} FSMComponent;

// --- Global Component Arrays ---
// These arrays store the actual component data, indexed by EntityID
extern PositionComponent            g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent            g_velocities[ECS_MAX_ENTITIES];
extern ColliderComponent            g_colliders[ECS_MAX_ENTITIES];
extern SpriteComponent              g_sprites[ECS_MAX_ENTITIES];
extern ScreenConstraintComponent    g_screen[ECS_MAX_ENTITIES];
extern FSMComponent                 g_fsm[ECS_MAX_ENTITIES];

#endif // _ENG_COMPONENTS_H_