#ifndef _ENG_ECS_H_
#define _ENG_ECS_H_

#include "config.h"
#include "components.h"

#include <genesis.h>

// Entity id definition
#if ECS_MAX_ENTITIES > 255
    typedef u16 EntityId;
#else
    typedef u8 EntityId;
#endif

// To keep track of active entities and their component compositions
extern bool g_entity_active[ECS_MAX_ENTITIES];
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES]; // Stores the combined flags for each entity

// --- Entity Management ---
void ECS_init();                    // Initialize all ECS data
void ECS_clearAllEntities();        // Mark all entities in memory as invalid and clear them.
//void ECS_update();                // Updates all registered subsystems

EntityId Entity_create();           // Get a new, active entity ID
void Entity_destroy(EntityId id);   // Deactivate an entity and "free" its components

// ... Component Management ...
void Entity_setComponentValue(EntityId id, ComponentType type, void *value);
void* Entity_getComponent(EntityId id, ComponentType type);
void Entity_addComponent(EntityId id, ComponentType type, void *value);
void Entity_removeComponent(EntityId id, ComponentType type);
bool Entity_hasComponent(EntityId id, ComponentType type); // Checks for a single component
bool Entity_hasAllComponents(EntityId id, ComponentMask types); // Checks for a set of components

// Add a position component to the specified entity
static inline void Entity_addSpriteComponent(EntityId entityId, const SpriteDefinition *spriteDef, u8 palette) {
    Entity_addComponent(entityId, COMPONENT_SPRITE, NULL);
    g_sprites[entityId].sgdkSprite = SPR_addSprite(
                             spriteDef,
                             0,                                     
                             0,                                     
                             TILE_ATTR(palette, TRUE, FALSE, FALSE) 
                         ); // Palette, priority, flip        
}

#endif // _ENG_ECS_H_