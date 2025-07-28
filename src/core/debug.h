#ifndef _ENG_DEBUG_H_
#define _ENG_DEBUG_H_

#include "config.h"

#if DEBUG_COLLIDER_BORDERS_ENABLED

#include "logger.h"

// SGDK forward declaration
#include <genesis.h>

// TODO: Evaluate the necessity of remove the entire file from 
// compilation then the collider debuggin is off

// Define a constant for the VRAM index of our debug tile
#define DEBUG_TILE_IDX (TILE_USER_INDEX)
// Sprite positionning offset
#define VDP_SPRITE_OFFSET_MIN_X 128
#define VDP_SPRITE_OFFSET_MIN_Y 128

/// A single 8x8 tile filled with color index 15 (0xF).
// Each u32 represents a row of 8 pixels, with each nibble (4 bits) being one pixel's color index.
extern const u32 solid_tile_data[8];

void DBG_init();
void DBG_update(Sprite* sprite, const SpriteDefinition* spriteDef, s16 previousX, s16 previousY);

// TODO: Evaluate the necessity of deleting these two functions They are deprecated in favor of DBG_drawBox.
// Declaration for our debug sprite drawing function
void DBG_drawSpriteBorder(Sprite* sprite, const SpriteDefinition* spriteDef, u16 tile_attr);
void DBG_clearSpriteBorder(Sprite* sprite, const SpriteDefinition* spriteDef);

// Declaration for our debug generic box sprite drawing function
void DBG_drawBox(s16 x, s16 y, u16 w, u16 h);
void DBG_clearBox(s16 x, s16 y, u16 w, u16 h);

// --- Update graphics ---
// Create a temporary sprite object at the *previous* position to clear the old border
#define DBG_CLEAR_SPRITE_BORDER(sprite, spriteDef) \
    Sprite temp_sprite_for_clearing; \
    temp_sprite_for_clearing.x = sprite->x; \
    temp_sprite_for_clearing.y = sprite->y; \
    DBG_clearSpriteBorder(&temp_sprite_for_clearing, spriteDef);

// Draw the new border at the current position
#define DBG_DRAW_SPRITE_BORDER(sprite, spriteDef) \
    DBG_drawSpriteBorder(sprite, spriteDef, TILE_ATTR(PAL1, 1, FALSE, FALSE));

// 2. Draw the new debug box at the CURRENT position
#define DBG_DRAW_SPRITE_BOX(sprite) \
    DBG_drawBox(sprite->x - VDP_SPRITE_OFFSET_MIN_X, sprite->y - VDP_SPRITE_OFFSET_MIN_Y, sprite->definition->w, sprite->definition->h)

#define DBG_CLEAR_SPRITE_BOX(sprite) \
    DBG_clearBox(sprite->x - VDP_SPRITE_OFFSET_MIN_X, sprite->y - VDP_SPRITE_OFFSET_MIN_Y, sprite->definition->w, sprite->definition->h)

// Calculate AABB bounds from the passed-in data.
// It's assumed pos is the top-left of the entity, and the collider's x/y is an offset.
#define DBG_DRAW_COLLIDER_BOX(collider, position) \
    DBG_drawBox( \
        F16_toRoundedInt (position->x) + collider->shape.box.x, \
        F16_toRoundedInt (position->y) + collider->shape.box.y, \
        collider->shape.box.w, \
        collider->shape.box.h); \

#define DBG_CLEAR_COLLIDER_BOX(collider, position) \
    DBG_clearBox( \
        F16_toRoundedInt (position->x) + collider->shape.box.x, \
        F16_toRoundedInt (position->y) + collider->shape.box.y, \
        collider->shape.box.w, \
        collider->shape.box.h); \

#else // DEBUG_COLLIDER_BORDERS_ENABLED

    // Define empty macros and stubs so the code compiles without logging
    #define DBG_init(DBG_init)

#endif

#endif // _ENG_DEBUG_H_