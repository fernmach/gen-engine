#include "debug.h"

const u32 solid_tile_data[8] =
{
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
};

void DBG_init() {
    // --- Debug Border Setup ---
    // 1. Set a bright color (e.g., bright red) for our border.
    // We use color 15 of palette 1 (PAL1). Sprites usually use PAL2 or PAL3.
    PAL_setColor(15 + 16, 0x000E); // Color 15 in PAL1, set to bright red

    // 2. Load the solid tile data into VRAM at our chosen index.
    VDP_loadTileData(solid_tile_data, DEBUG_TILE_IDX, 1, DMA);

    LOGGER_INFO("Debug subsystem initialized.");
}

void DBG_update(Sprite* sprite, const SpriteDefinition* spriteDef, s16 newX, s16 newY) {
    // --- Update graphics ---
    // Create a temporary sprite object at the *previous* position to clear the old border
    Sprite temp_sprite_for_clearing;
    temp_sprite_for_clearing.x = sprite->x;
    temp_sprite_for_clearing.y = sprite->y;
    DBG_clearSpriteBorder(&temp_sprite_for_clearing, spriteDef);

    // Draw the new border at the current position
    const u16 border_tile_attr = TILE_ATTR(PAL1, 1, FALSE, FALSE); // High priority, PAL1
    DBG_drawSpriteBorder(sprite, spriteDef, border_tile_attr);  
}

/**
 * @brief Draws a border on BG_A around a sprite's bounding box.
 *
 * @param sprite The sprite to draw a border around.
 * @param spriteDef The sprite's definition, needed to get its size.
 * @param tile_attr The tile attribute to use for drawing (should contain palette and priority).
 */
void DBG_drawSpriteBorder(Sprite* sprite, const SpriteDefinition* spriteDef, u16 tile_attr)
{
    // SGDK sprite positions are offset by +128. We convert them back to screen pixels.
    const s16 x = sprite->x - VDP_SPRITE_OFFSET_MIN_X;
    const s16 y = sprite->y - VDP_SPRITE_OFFSET_MIN_Y;

    // Get sprite size in pixels from its definition
    const u16 w = spriteDef->w;
    const u16 h = spriteDef->h;

    // Convert pixel coordinates and dimensions to tile coordinates
    const s16 x_tile = x / 8;
    const s16 y_tile = y / 8;

    // Round up the width/height to the nearest tile
    const u16 w_tile = (w + 7) / 8;
    const u16 h_tile = (h + 7) / 8;

    // Use our solid-color debug tile
    const u16 tile_index = TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, DEBUG_TILE_IDX);

    // Draw the 4 sides of the rectangle
    // Top border
    VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile, w_tile, 1);
    // Bottom border
    VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile + h_tile - 1, w_tile, 1);
    // Left border
    VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile + 1, 1, h_tile - 2);
    // Right border
    VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile + w_tile - 1, y_tile + 1, 1, h_tile - 2);
}

/**
 * @brief Clears a border on BG_A by drawing with the empty tile (index 0).
 */
void DBG_clearSpriteBorder(Sprite* sprite, const SpriteDefinition* spriteDef)
{
    // Re-calculate the previous frame's border position and size
    const s16 x = sprite->x - VDP_SPRITE_OFFSET_MIN_X;
    const s16 y = sprite->y - VDP_SPRITE_OFFSET_MIN_Y;
    const u16 w = spriteDef->w;
    const u16 h = spriteDef->h;
    const s16 x_tile = x / 8;
    const s16 y_tile = y / 8;
    const u16 w_tile = (w + 7) / 8;
    const u16 h_tile = (h + 7) / 8;

    // Clear the area using the empty tile (index 0 with default attributes)
    VDP_clearTileMapRect(VDP_BG_A, x_tile, y_tile, w_tile, h_tile);
}

/**
 * @brief Draws a hollow rectangular border on BG_A for debugging.
 *        This is a generic function that uses logical screen coordinates.
 *
 * @param x         The logical X position (in pixels) of the box's top-left corner.
 * @param y         The logical Y position (in pixels) of the box's top-left corner.
 * @param w         The width of the box in pixels.
 * @param h         The height of the box in pixels.
 */
void DBG_drawBox(s16 x, s16 y, u16 w, u16 h)
{
    // Convert pixel coordinates and dimensions to tile coordinates
    const s16 x_tile = x / 8;
    const s16 y_tile = y / 8;
    // Use ceiling division to ensure the box covers the entire pixel area
    const u16 w_tile = (w + 7) / 8;
    const u16 h_tile = (h + 7) / 8;

    // Build the tile attribute for our debug tile.
    // Uses DEBUG_TILE_IDX, PAL1, and high priority.
    const u16 tile_index = TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, DEBUG_TILE_IDX);

    // Don't draw if the box has no size
    if (w_tile == 0 || h_tile == 0) {
        return;
    }

    // Draw the 4 sides of the rectangle
    // Top border
    VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile, w_tile, 1);
    // Bottom border
    if (h_tile > 1) {
        VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile + h_tile - 1, w_tile, 1);
    }
    // Left and Right borders (only if box is thick enough)
    if (h_tile > 2) {
        if (w_tile > 1) {
            // Right border
            VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile + w_tile - 1, y_tile + 1, 1, h_tile - 2);
        }
        // Left border
        VDP_fillTileMapRect(VDP_BG_A, tile_index, x_tile, y_tile + 1, 1, h_tile - 2);
    }
}

/**
 * @brief Clears a rectangular area on BG_A.
 *
 * @param x         The logical X position (in pixels) of the box's top-left corner.
 * @param y         The logical Y position (in pixels) of the box's top-left corner.
 * @param w         The width of the box in pixels.
 * @param h         The height of the box in pixels.
 */
void DBG_clearBox(s16 x, s16 y, u16 w, u16 h)
{
    // Convert pixel coordinates and dimensions to tile coordinates
    const s16 x_tile = x / 8;
    const s16 y_tile = y / 8;
    const u16 w_tile = (w + 7) / 8;
    const u16 h_tile = (h + 7) / 8;

    // Clear the entire rectangular area by filling it with the empty tile (index 0)
    VDP_clearTileMapRect(VDP_BG_A, x_tile, y_tile, w_tile, h_tile);
}