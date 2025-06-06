#include "scene_menu.h"

// If you need to access other scenes (e.g., to switch to scene_main)

// Main game scene
// Needed for transitioning
//Scene main_game_scene;

// --- Menu Scene Specific Data (Optional) ---
typedef struct {
    s16 option_selected;
    // Add other menu specific data here
    // e.g., Sprite* title_sprite;
} MenuSceneData;

// Statically allocate data for this scene
static MenuSceneData menu_data;

// --- Menu Scene Lifecycle Functions ---

static void menu_init(Scene* scene_self) {
    // scene_self->data will point to menu_data
    MenuSceneData* data = (MenuSceneData*)scene_self->data;
    data->option_selected = 0;

    VDP_drawText("Main Menu", 10, 10);
    VDP_drawText("Press A for Game", 10, 12);
    VDP_drawText("Press B for Options (nyi)", 10, 14);

    // Load menu specific resources (tiles, sprites, palettes, music)
    // e.g., SPR_init(); XGM_startPlay(my_menu_music);
    // VDP_loadTileSet(menu_tiles, TILE_USER_INDEX, DMA);
    // PAL_setPalette(PAL0, menu_palette_bg);
    // PAL_setPalette(PAL1, menu_palette_spr);

    KLog("Menu Scene: Init");
}

static void menu_update(Scene* scene_self, fix16 dt) {
    // scene_self->data will point to menu_data
    //MenuSceneData* data = (MenuSceneData*)scene_self->data;
    if (Input_isJustPressed(JOY_1, BUTTON_A)) {
        LOGGER_DEBUG("Menu Scene: Start pressed, switching to Game Scene");
        SceneManager_setNextScene(&main_scene); // Switch to game scene
    }

    if (Input_isJustPressed(JOY_1, BUTTON_B)) {
        // Potentially switch to an options scene or handle in-menu
        LOGGER_DEBUG("Menu Scene: A pressed");        
    }

    // Update menu logic, animations, etc.
}

static void menu_draw(Scene* scene_self) {
    // Draw menu elements
    // e.g., SPR_update();
    // VDP_waitVSync(); // Already handled in main loop usually by SYS_doVBlankProcess()
}

static void menu_destroy(Scene* scene_self) {
    MenuSceneData* data = (MenuSceneData*)scene_self->data;

    // Clear all entitities before leaving the scene
    //ECS_clearAllEntities();

    // Resets the ball count to 0
    //data->ballsCount = 0; // Reset scene data
    data->option_selected = 0;

    // Reset all sprites
    //SPR_reset();

    // Clear planes before leave
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);

    LOGGER_DEBUG("Menu Scene: destroyed");
    // Unload menu specific resources
    // e.g., SPR_reset(); XGM_stopPlay();
    // VDP_clearPlane(BG_A, TRUE); VDP_clearPlane(BG_B, TRUE);
    // PAL_fadeOutAll(15, FALSE); // Example fade out
}

// --- Menu Scene Definition ---
// Needs to be accessible by main.c or scene_manager if it sets the first scene
Scene menu_scene = {
    .init = menu_init,
    .update = menu_update,
    .draw = menu_draw,
    .destroy = menu_destroy,
    .data = &menu_data, // Point to our static data
    .name = "MenuScene"
};