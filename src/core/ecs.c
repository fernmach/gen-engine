#include "config.h"
#include "defines.h"
#include "ecs.h"
#include "logger.h"
#include "asserts.h"
#include "components.h"

#define ASSERT_MAX_ENTITY(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, Max: %u", #method, id, ECS_MAX_ENTITIES);*/ \
    ASSERT_MSG(id < ECS_MAX_ENTITIES, "ECS: Entity id out of range at " #method);

#define ASSERT_INACTIVE_ENTITY(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, Active: %u", #method, id, g_entity_active[id]);*/ \
    ASSERT_MSG(id < g_active_entity_count, "ECS: Inactive entity id provided at " #method);

#define ASSERT_COMPONENT_TYPE(method) \
    /*LOGGER_DEBUG("ECS: %s. Entity: %d, type: %u", #method, id, type);*/ \
    ASSERT_MSG(type > 0 && type < ECS_MAX_COMPONENTS, "ECS: Inactive component type provided at" #method);

#define ASSERT_ALL(method) \
    ASSERT_MAX_ENTITY (method) \
    ASSERT_INACTIVE_ENTITY (method) \
    ASSERT_COMPONENT_TYPE (method) \

// --- Global component array definitions ---
PositionComponent           g_positions[ECS_MAX_ENTITIES];
VelocityComponent           g_velocities[ECS_MAX_ENTITIES];
ColliderComponent           g_colliders[ECS_MAX_ENTITIES];
SpriteComponent             g_sprites[ECS_MAX_ENTITIES];
ScreenConstraintComponent   g_screen[ECS_MAX_ENTITIES];
FSMComponent                g_fsm[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
//bool g_entity_active[ECS_MAX_ENTITIES];
ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

// --- NEW: Internal State for Packed Array ---
// Tracks the number of active entities for the swap and pop operation
// All active entities are guaranteed to be in the range [0, g_active_entity_count - 1].
EntityId g_active_entity_count = 0;

void ECS_init() {
    // Zero out all component data and entity states
    memset(g_positions, 0, sizeof(g_positions));
    memset(g_velocities, 0, sizeof(g_velocities));
    memset(g_colliders, 0, sizeof(g_colliders));
    memset(g_sprites, 0, sizeof(g_sprites));
    memset(g_screen, 0, sizeof(g_screen));
    memset(g_fsm, 0, sizeof(g_fsm));

    // Initialize all masks to 0
    memset(g_entity_component_masks, COMPONENT_NONE, sizeof(g_entity_component_masks));

    g_active_entity_count = 0;

    // Important: Initialize SGDK Sprite engine before starting ECS (SPR_init())
    // This is implemente in the Engine_init() event
    LOGGER_INFO("ECS subsystem initialized");
}

void ECS_clearAllEntities() {
    for (EntityId i = 0; i < g_active_entity_count; ++i) {
        // Clear any resources if exists before clearing entity
        if ((g_entity_component_masks[i] & COMPONENT_SPRITE) != 0) {
            if (g_sprites[i].sgdkSprite != NULL) {
                SPR_releaseSprite(g_sprites[i].sgdkSprite);
                g_sprites[i].sgdkSprite = NULL;
            }
        }
    }    

    // Zero out all component data and entity states
    memset(g_positions, 0, sizeof(g_positions));
    memset(g_velocities, 0, sizeof(g_velocities));
    memset(g_colliders, 0, sizeof(g_colliders));
    memset(g_sprites, 0, sizeof(g_sprites));
    memset(g_screen, 0, sizeof(g_screen));
    memset(g_fsm, 0, sizeof(g_fsm));
    memset(g_entity_component_masks, COMPONENT_NONE, sizeof(g_entity_component_masks));

    g_active_entity_count = 0;
    LOGGER_INFO("ECS: Clearing all entities");
}

EntityId Entity_create() {    
    if (g_active_entity_count >= ECS_MAX_ENTITIES) {
        LOGGER_WARN("ECS: Maximum number of entites reached %d", ECS_MAX_ENTITIES);
        return ECS_MAX_ENTITIES;
    }

    EntityId new_id = g_active_entity_count;

    // Reset its mask. The component data is already considered garbage from previous use,
    // and will be overwritten when components are added.
    // Optional: Clear component data for this entity
    // The new entity ID is simply the current count.
    g_entity_component_masks[new_id] = COMPONENT_NONE;

    g_active_entity_count++;

    //LOGGER_DEBUG("ECS: Created entity %d", new_id);

    return new_id;
}

void Entity_destroy(EntityId id) {   
    ASSERT_MAX_ENTITY("Entity_destroy");
    ASSERT_INACTIVE_ENTITY("Entity_destroy");

    // --- Component-specific cleanup (IMPORTANT) ---
    // If a component needs special cleanup (like releasing an SGDK sprite),
    // do it BEFORE you overwrite its data with the swap.
    if ((g_entity_component_masks[id] & COMPONENT_SPRITE) != 0) {
        if (g_sprites[id].sgdkSprite != NULL) {
            SPR_releaseSprite(g_sprites[id].sgdkSprite);
            g_sprites[id].sgdkSprite = NULL;
        }
    }
    // --- End cleanup ---

    // Decrement the active entity count. This is the "pop".
    g_active_entity_count--;    

    // Get the ID of the last active entity.
    EntityId last_id = g_active_entity_count;

    // If the entity we are destroying is NOT the last one in the array,
    // we need to move the last one into its place to fill the gap.
    if (id != last_id) {
        // This is the "swap" part.
        g_positions[id]                     = g_positions[last_id];
        g_velocities[id]                    = g_velocities[last_id];
        g_colliders[id]                     = g_colliders[last_id];
        g_sprites[id]                       = g_sprites[last_id];
        g_screen[id]                        = g_screen[last_id];
        g_fsm[id]                           = g_fsm[last_id];
        // TODO: Implement tagging?
        // IMPORTANT: If any external system holds a reference to the entity that was at 'last_id',
        // that reference is now invalid. You must have a way to update it.
        // For example, if the player was entity 'last_id', you now need to know it's at 'id'.
        //LOGGER_DEBUG("ECS: Destroyed entity %d, swaped for %d", id, last_id);        
    }    
    g_entity_component_masks[last_id]   = COMPONENT_NONE; // for the last position
    //LOGGER_DEBUG("ECS: current active count %d", g_active_entity_count); 
}

void Entity_addComponent(EntityId id, ComponentType type) {
    ASSERT_ALL("Entity_addComponent");
    
    g_entity_component_masks[id] |= type;   
}

void Entity_removeComponent(EntityId id, ComponentType type) {
    ASSERT_ALL("Entity_removeComponent");

    g_entity_component_masks[id] &= ~type;
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

// void ECS_getStatsFromComponentArray(void* arr[], ComponentType type) {
//     switch (type)
//     {
//     case COMPONENT_POSITION:
//         for (int i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
//             if ( (PositionComponent*)arr[i] )
//         }
//         break;
    
//     default:
//         break;
//     }
// }

// void ECS_DrawStats(u16 x, u16 y) {
//     char buffer[40]; // Max SGDK line width
//     VDP_setTextPalette(0);

//     sprintf(buffer, "ECS Max, %d, Active: %d, ", ECS_MAX_ENTITIES, ACTIVE_ENTITY_COUNT);
//     VDP_drawText(buffer, x, y++);
//     //VDP_drawText("Name         TotFr Call MinC MaxC AvgC AvgFr", x, y++);
//     // Col Widths:   12           6      3    4    4    4    5   = 38 + spaces
//     VDP_drawText("Comp         TotFr MinC MaxC AvgC AvgFr", x, y++);
//     // Col Widths:   12           6      3    4    4    4    5   = 38 + spaces

//     for (int i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {

//         if (profilerEntries[i].active) {
//             ProfilerEntry* entry = &profilerEntries[i];

//             u16 name = ;
//             u32 us_min_call_curr = (entry->minSubTicksPerCallThisFrame == MAX_U32) ? 0 : PROFILER_SUBTICKS_TO_US(entry->minSubTicksPerCallThisFrame);
//             u32 us_max_call_curr = PROFILER_SUBTICKS_TO_US(entry->maxSubTicksPerCallThisFrame);
//             u32 us_avg_call = PROFILER_SUBTICKS_TO_US(entry->avgSubTicksPerCall);
//             u32 us_avg_frame = PROFILER_SUBTICKS_TO_US(entry->avgSubTicksPerFrame);


//             // Make sure sprintf and buffer can handle the numbers.
//             // %lu should be fine for u32. If us_total_frame > ~4.2M it overflows u32.
//             // Max frame time in subticks: (1/60s) * 76800 subticks/s = 1280 subticks for NTSC.
//             // Max us: 1280 * 625 / 48 = 16666 us. Fits u32.
//             // sprintf(buffer, "%-*.*s %6lu %3lu %4lu %4lu %4lu %5lu",
//             //         PROFILER_NAME_MAX_LEN, PROFILER_NAME_MAX_LEN, entry->name,
//             //         us_total_frame,
//             //         entry->callCountThisFrame,
//             //         us_min_call_curr,
//             //         us_max_call_curr,
//             //         us_avg_call,
//             //         us_avg_frame);
//             sprintf(buffer, "%-*.*s %5lu %4lu %4lu %4lu %5lu",
//                     PROFILER_NAME_MAX_LEN-3, PROFILER_NAME_MAX_LEN-3, entry->name,
//                     us_total_frame,                    
//                     us_min_call_curr,
//                     us_max_call_curr,
//                     us_avg_call,
//                     us_avg_frame);
//             VDP_drawText(buffer, x, y++);
//             if (y >= (VDP_getScreenHeight() / 8) -1 ) break;
//         }
//     }
// }
