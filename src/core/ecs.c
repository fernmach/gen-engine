#include "config.h"
#include "ecs.h"
#include "logger.h"
#include "asserts.h"
#include "components.h"

#define ASSERT_MAX_ENTITY(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, Max: %u", #method, id, ECS_MAX_ENTITIES);*/ \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at " #method);

#define ASSERT_INACTIVE_ENTITY(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, Active: %u", #method, id, g_entity_active[id]);*/ \
    ASSERT_MSG(g_entity_active[id], "ECS: Inactive entity id provided at " #method);

#define ASSERT_COMPONENT_TYPE(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, type: %u", #method, id, type);*/ \
    ASSERT_MSG(type > 0 && type < ECS_MAX_COMPONENTS, "ECS: Inactive entity id provided at" #method);

#define ASSERT_ALL(method) \
    ASSERT_MAX_ENTITY (method) \
    ASSERT_INACTIVE_ENTITY (method) \
    ASSERT_COMPONENT_TYPE (method) \

//LOGGER_FATAL("ECS: Error while removing component value. Entity: %d, Component: %u", id, type);

// --- Global component array definitions ---
PositionComponent           g_positions[ECS_MAX_ENTITIES];
VelocityComponent           g_velocities[ECS_MAX_ENTITIES];
SpriteComponent             g_sprites[ECS_MAX_ENTITIES];
ScreenConstraintComponent   g_screen[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
bool g_entity_active[ECS_MAX_ENTITIES];
ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// --- Holds the next available entity id in the pool---
static EntityId s_next_entity_id_pool = 0;

void ECS_init() {
    // Zero out all component data and entity states
    memset(g_positions, 0, sizeof(g_positions));
    memset(g_velocities, 0, sizeof(g_velocities));
    memset(g_sprites, 0, sizeof(g_sprites));
    memset(g_screen, 0, sizeof(g_screen));

    memset(g_entity_active, FALSE, sizeof(g_entity_active));

     // Initialize all masks to 0
    memset(g_entity_component_masks, COMPONENT_NONE, sizeof(g_entity_component_masks));

    s_next_entity_id_pool = 0;

    // Important: Initialize SGDK Sprite engine before starting ECS (SPR_init())
    // This is implemente in the Engine_init() event
    LOGGER_INFO("ECS subsystem initialized");
}

void ECS_clearAllEntities() {
    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i]) {
           Entity_destroy(i);
        }
    }
}

EntityId Entity_create() {
    // Loop way
    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (!g_entity_active[i]) {
            g_entity_active[i] = TRUE;
            g_entity_component_masks[i] = COMPONENT_NONE;
            // Optional: Clear component data for this entity
            return i;
        }
    }

    // -- POOL WAY --
    // if (s_next_entity_id_pool <= ECS_MAX_ENTITIES) {
    //     g_entity_active[s_next_entity_id_pool] = TRUE;
    //     g_entity_component_masks[s_next_entity_id_pool] = COMPONENT_NONE;
    //     return s_next_entity_id_pool++;
    // }

    // Only reached when maximmum reached
    LOGGER_WARN("ECS: Maximum number of entites reached %d", ECS_MAX_ENTITIES);
    return ECS_MAX_ENTITIES;
}

void Entity_destroy(EntityId id) {   
    ASSERT_MAX_ENTITY("Entity_destroy");
    ASSERT_INACTIVE_ENTITY("Entity_destroy");

    g_entity_active[id] = FALSE;
    g_entity_component_masks[id] = COMPONENT_NONE; // Clear the mask

    /*if (g_sprites[id].sgdk_sprite != NULL) { // Example of component-specific cleanup
        SPR_releaseSprite(g_sprites[id].sgdk_sprite);
        g_sprites[id].sgdk_sprite = NULL;
    }*/
    // Note: The actual data in g_positions[id], g_velocities[id] etc.
    // is still there but considered "garbage" because the mask says
    // the entity no longer "has" those components.
    //}        
    
    // -- POOL WAY --
    // if (g_entity_active[id]) {
    //     // Finde the head of the entities array
    //     EntityId head_entity = s_next_entity_id_pool-1;

    //     // Transfer the head to the newly available position
    //     g_positions[id] = g_positions[head_entity];
    //     g_velocities[id] = g_velocities[head_entity];
    //     g_sprites[id] = g_sprites[head_entity];

    //     // Decactivate the head
    //     g_entity_active[head_entity] = FALSE;
    //     g_entity_component_masks[head_entity] = COMPONENT_NONE; // Clear the mask

    //     s_next_entity_id_pool -= 1;
    // } 
}

void Entity_setComponentValue(EntityId id, ComponentType type, void* values) {
    ASSERT_ALL("Entity_setComponentValue");
    ASSERT_MSG(values != NULL, "ECS: Parameter 'values' required");

    if(type == COMPONENT_POSITION) {
        // Cast 'values' to a PositionComponent pointer and dereference it
        // This copies the *contents* of the PositionComponent struct
        // pointed to by 'values' into g_positions[id].
        //LOGGER_DEBUG("INSIDE component %d", ((PositionComponent*)values)->x);
        //ASSERT_EXP(1!=0) ;
        g_positions[id] = *(PositionComponent*)values;
        return;
    }

    if(type == COMPONENT_VELOCITY) {            
        g_velocities[id] = *(VelocityComponent*)values;
        return;
    }

    if(type == COMPONENT_SPRITE) {
        g_sprites[id] = *(SpriteComponent*)values;
        return;
    }    

    if(type == COMPONENT_SCREEN_CONSTRAINT) {
        g_screen[id] = *(ScreenConstraintComponent*)values;
        return;
    }    
}

void* Entity_getComponent(EntityId id, ComponentType type) {
    ASSERT_ALL("Entity_getComponent");
    //ASSERT_MSG(Entity_hasComponent(id, type),
    //    "ECS: Component not found in entity");

    // Is the component attached to the entity (Entity_hasComponent)
    if ((g_entity_component_masks[id] & type) != 0) {
        if(type == COMPONENT_POSITION) {        
            return (void*)&g_positions[id];
        }

        if(type == COMPONENT_VELOCITY) {
            return (void*)&g_velocities[id];
        }

        if(type == COMPONENT_SPRITE) {
            return (void*)&g_sprites[id];
        }

        if(type == COMPONENT_SCREEN_CONSTRAINT) {
            return (void*)&g_screen[id];
        }
    }

    return NULL;
}

void Entity_addComponent(EntityId id, ComponentType type, void* value) {
    ASSERT_ALL("Entity_addComponent");

    //if (id < ECS_MAX_ENTITIES && g_entity_active[id]) {
    g_entity_component_masks[id] |= type;

    if (value != NULL) {
        Entity_setComponentValue(id, type, value);
    }

    // Note: You might want to initialize the component data here
    // e.g., if (component_mask == COMPONENT_POSITION) { memset(&g_positions[id], 0, sizeof(PositionComponent)); }
    // Or, more typically, the code that calls addComponent is responsible for also setting the data.    
}

void Entity_removeComponent(EntityId id, ComponentType type) {
    ASSERT_ALL("Entity_removeComponent");

    g_entity_component_masks[id] &= ~type;

    // Optional: Zero out component data (g_positions[id] = {0,0};)
    // This helps if you're worried about stale data, but the mask prevents its use.
    // Is the component attached to the entity (Entity_hasComponent)    
    if(type == COMPONENT_POSITION) {        
        g_positions[id] = (PositionComponent){0, 0};
        return;
    }

    if(type == COMPONENT_VELOCITY) {
        g_velocities[id] = (VelocityComponent){0, 0};
        return;
    }

    if(type == COMPONENT_SPRITE) {            
        SPR_releaseSprite(g_sprites[id].sgdkSprite);
        g_sprites[id].sgdkSprite = NULL;
        return;
    }

    if(type == COMPONENT_SCREEN_CONSTRAINT) {
        g_screen[id] = (ScreenConstraintComponent){FALSE,FALSE};
        return;
    }
}

// Checks if an entity has AT LEAST ONE of the flags in 'component_mask_query'
// (Usually you want to check for a single specific flag)
bool Entity_hasComponent(EntityId id, ComponentType type_query) {    
    ASSERT_MAX_ENTITY("Entity_hasComponent");
    ASSERT_INACTIVE_ENTITY("Entity_hasComponent");

    return (g_entity_component_masks[id] & type_query) != 0;
}

// Checks if an entity has ALL the component flags set in 'required_mask'
bool Entity_hasAllComponents(EntityId id, ComponentMask required_mask) {
    ASSERT_MAX_ENTITY("Entity_hasAllComponents");
    ASSERT_INACTIVE_ENTITY("Entity_hasAllComponents");
        
    return (g_entity_component_masks[id] & required_mask) == required_mask;    
}