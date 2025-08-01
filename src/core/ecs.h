#ifndef _ENG_ECS_H_
#define _ENG_ECS_H_

#include "config.h"
#include "defines.h"
#include "asserts.h"
#include "components.h"

#include <genesis.h>

//#define INVALID_ENTITY_ID ((EntityId)-1)

// To keep track of active entities and their component compositions
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES]; // Stores the combined flags for each entity
extern EntityId g_active_entity_count;

// --- Entity Management ---
void ECS_init();                    // Initialize all ECS data
void ECS_clearAllEntities();        // Mark all entities in memory as invalid and clear them.
//void ECS_update();                // Updates all registered subsystems

EntityId Entity_create();           // Get a new, active entity ID
void Entity_destroy(EntityId id);   // Deactivate an entity and "free" its components

// ... Component Management ...
void Entity_addComponent(EntityId id, ComponentType type);
void Entity_removeComponent(EntityId id, ComponentType type);
bool Entity_hasComponent(EntityId id, ComponentType type); // Checks for a single component
bool Entity_hasAllComponents(EntityId id, ComponentMask required_mask); // Checks for a set of components

// Add a position component to the specified entity
static inline void Entity_addComponentSprite(EntityId id, const SpriteDefinition *spriteDef, u8 palette) {    
    g_entity_component_masks[id] |= COMPONENT_SPRITE; 
    g_sprites[id].sgdkSprite = SPR_addSprite(
                             spriteDef,
                             0,                                     
                             0,                                     
                             TILE_ATTR(palette, TRUE, FALSE, FALSE) 
                         ); // Palette, priority, flip
    g_sprites[id].animationId = UNDEFINED_S16;
    g_sprites[id].depth = UNDEFINED_S16;
    g_sprites[id].visibility = UNDEFINED_U16;
    g_sprites[id].hFlip = UNDEFINED_BOOL;
    g_sprites[id].vFlip = UNDEFINED_BOOL;
}

// Add a position component to the specified entity
static inline SpriteComponent* Entity_getComponentSprite(EntityId id) {
    return &g_sprites[id];
}

/*---- Direct Sprite component manipulation ------*/
// Add a position component to the specified entity
static inline void SpriteComponent_setAnimation(SpriteComponent* sprite, s16 animationId) {    
    sprite->animationId = animationId;
}

static inline void SpriteComponent_setHFlip(SpriteComponent* sprite, bool hFlip) {    
    sprite->hFlip = hFlip;
}

static inline void SpriteComponent_setVFlip(SpriteComponent* sprite, bool vFlip) {    
    sprite->hFlip = vFlip;
}

static inline void SpriteComponent_setDepth(SpriteComponent* sprite, s16 value) {    
    sprite->depth = value;
}

static inline void SpriteComponent_setVisibility(SpriteComponent* sprite, u16 value) {    
    sprite->visibility = value;
}

/*---- Entity sprite component manipulation ------*/
// Add a position component to the specified entity
static inline void Entity_setComponentSpriteAnimation(EntityId id, s16 animationId) {
    SpriteComponent_setAnimation(&g_sprites[id], animationId);    
}

static inline void Entity_setComponentSpriteHFlip(EntityId id, bool hFlip) {
    SpriteComponent_setHFlip(&g_sprites[id], hFlip);
}

static inline void Entity_setComponentSpriteVFlip(EntityId id, bool vFlip) {
    SpriteComponent_setVFlip(&g_sprites[id], vFlip);
}

static inline void Entity_setComponentSpriteDepth(EntityId id, s16 value) {
    SpriteComponent_setDepth(&g_sprites[id], value);
}

static inline void Entity_setComponentSpriteVisibility(EntityId id, s16 value) {
    SpriteComponent_setVisibility(&g_sprites[id], value);
}

// To use outsisde of the ecs
#define ACTIVE_ENTITY_COUNT g_active_entity_count

#define REGISTER_COMPONENT_ADD_ACCESSOR(name, entityArrayName, componentType, typeDataStruct) \
static inline void Entity_addComponent##name(EntityId id, typeDataStruct value) { \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at Entity_addComponent"#name); \
    ASSERT_MSG(id < g_active_entity_count, "ECS: Inactive entity id provided at Entity_addComponent"#name); \
    ASSERT_MSG(componentType > 0 && componentType < COMPONENT_MAX_VALUE, "ECS: Invalid component type provided at Entity_addComponent"#name); \
    g_entity_component_masks[id] |= componentType; \
    entityArrayName[id] = value; \
}

#define REGISTER_COMPONENT_REMOVE_ACCESSOR(name, componentType) \
static inline void Entity_removeComponent##name(EntityId id) { \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at Entity_removeComponent"#name); \
    ASSERT_MSG(id < g_active_entity_count, "ECS: Inactive entity id provided at Entity_removeComponent"#name); \
    ASSERT_MSG(componentType > 0 && componentType < COMPONENT_MAX_VALUE, "ECS: Invalid component type provided at Entity_removeComponent"#name); \
    g_entity_component_masks[id] &= ~componentType; \
}

// ASSERT_MSG((g_entity_component_masks[id] & componentType) = 0, "ECS: Component not attached to entity  Entity_getComponent"#name);
#define REGISTER_COMPONENT_GET_ACCESSOR(name, entityArrayName, componentType, typeDataStruct) \
static inline typeDataStruct* Entity_getComponent##name(EntityId id) { \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at Entity_getComponent"#name); \
    ASSERT_MSG(id < g_active_entity_count, "ECS: Inactive entity id provided at Entity_getComponent"#name); \
    ASSERT_MSG(componentType > 0 && componentType < COMPONENT_MAX_VALUE, "ECS: Invalid component type provided at Entity_getComponent"#name); \
    return &entityArrayName[id]; \
}

#define REGISTER_COMPONENT_SET_ACCESSOR(name, entityArrayName, componentType, typeDataStruct) \
static inline void Entity_setComponent##name(EntityId id, typeDataStruct* value) { \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at Entity_setComponent"#name); \
    ASSERT_MSG(id < g_active_entity_count, "ECS: Inactive entity id provided at Entity_setComponent"#name); \
    ASSERT_MSG(componentType > 0 && componentType < COMPONENT_MAX_VALUE, "ECS: Invalid component type provided at Entity_setComponent"#name); \
    entityArrayName[id] = *value; \
}

#define REGISTER_COMPONENT_ACESSORS(name, entityArrayName, componentType, typeDataStruct) \
    REGISTER_COMPONENT_ADD_ACCESSOR(name, entityArrayName, componentType, typeDataStruct) \
    REGISTER_COMPONENT_REMOVE_ACCESSOR(name, componentType) \
    REGISTER_COMPONENT_GET_ACCESSOR(name, entityArrayName, componentType, typeDataStruct) \
    REGISTER_COMPONENT_SET_ACCESSOR(name, entityArrayName, componentType, typeDataStruct)    

REGISTER_COMPONENT_ACESSORS(Position, g_positions, COMPONENT_POSITION, PositionComponent);
REGISTER_COMPONENT_ACESSORS(Velocity, g_velocities, COMPONENT_VELOCITY, VelocityComponent);
REGISTER_COMPONENT_ACESSORS(Collider, g_colliders, COMPONENT_COLLIDER, ColliderComponent);
REGISTER_COMPONENT_ACESSORS(ScreenConstraint, g_screen, COMPONENT_SCREEN_CONSTRAINT, ScreenConstraintComponent);
REGISTER_COMPONENT_ACESSORS(FSM, g_fsm, COMPONENT_FSM, FSMComponent);

#endif // _ENG_ECS_H_