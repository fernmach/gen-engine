#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"

#include <genesis.h>

// --- Global component array definitions ---
extern PositionComponent       g_positions[ECS_MAX_ENTITIES];
extern VelocityComponent       g_velocities[ECS_MAX_ENTITIES];
extern SpriteComponent         g_sprites[ECS_MAX_ENTITIES];

// --- Global entity tracking array definitions  ---
extern bool g_entity_active[ECS_MAX_ENTITIES];
extern ComponentMask g_entity_component_masks[ECS_MAX_ENTITIES];

void MovementSystem_update(fix16 dt) {
    // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_VELOCITY;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {            
            g_positions[i].x += F16_mul(g_velocities[i].dx, dt);
            g_positions[i].y += F16_mul(g_velocities[i].dy, dt);
            //LOGGER_DEBUG("Running %d %d", g_positions[i].x, g_positions[i].y);
        }
    }
}

void ScreenConstraintSystem_update() {
     // Define the components this system operates on
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE |COMPONENT_SCREEN_CONSTRAINT;

    const u16 screen_width = VDP_getScreenWidth();
    const u16 screen_height = VDP_getScreenHeight();
    //const u16 screen_width = 320;
    //const u16 screen_height = 224;

    //LOGGER_DEBUG("%d, %d", screen_width, screen_height);

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {

            // ... (screen bounds logic) ...
            //s16 screen_x = F16_toInt(g_positions[i].x);
            //s16 screen_y = F16_toInt(g_positions[i].y);
            
            //screen x
            if (F16_toInt(g_positions[i].x) < 0) {                
                g_positions[i].x = FIX16(0);
                g_velocities[i].dx = -g_velocities[i].dx;                
            }
            
            //screen y
            if (F16_toInt(g_positions[i].y) < 0) {                
                g_positions[i].y = FIX16(0);
                g_velocities[i].dy = -g_velocities[i].dy;
            }

            if ((F16_toInt(g_positions[i].x) + g_sprites[i].sgdkSprite->definition->w) > screen_width) {                    
                g_positions[i].x = FIX16(screen_width - g_sprites[i].sgdkSprite->definition->w);                    
                g_velocities[i].dx = -g_velocities[i].dx;                    
            }

            if ((F16_toInt(g_positions[i].y) + g_sprites[i].sgdkSprite->definition->h) > screen_height) {
                g_positions[i].y = FIX16(screen_height - g_sprites[i].sgdkSprite->definition->h);
                g_velocities[i].dy = -g_velocities[i].dy;
            }
        }
    }
}

void RenderSystem_update() {
    // This system needs Position (to know where) and Sprite (to know what)
    const ComponentMask required_mask = COMPONENT_POSITION | COMPONENT_SPRITE;

    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        if (g_entity_active[i] && Entity_hasAllComponents(i, required_mask)) {
            if (g_sprites[i].sgdkSprite != NULL) { // Still good to check the actual sprite pointer

                //LOGGER_DEBUG("Entity %d position {%d, %d}", i, g_positions[i].x, g_positions[i].y);
                //LOGGER_DEBUG("Entity %d screen {%d, %d}", i, screen_x, screen_y);

                // Using F16_toInt would be the correct way to go but the numbers
                // are beeing incorrectly retured.
                s16 screen_x = F16_toInt(g_positions[i].x);
                s16 screen_y = F16_toInt(g_positions[i].y);

                SPR_setPosition(g_sprites[i].sgdkSprite, screen_x, screen_y);        
                // ... other sprite updates ...

                // Update animation, visibility, etc. based on SpriteComponent data
                // SPR_setAnim(g_sprites[i].sgdk_sprite, g_sprites[i].current_anim_frame);
                // SPR_setVisibility(g_sprites[i].sgdk_sprite, VISIBLE / HIDDEN);
            }
        }
    }
}