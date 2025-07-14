#include "scene_fsm.h"
#include "fsm_player.h"

// If you need to access other scenes (e.g., to switch to scene_main)

// Main game scene
// Needed for transitioning
//Scene main_game_scene;

// --- Menu Scene Specific Data (Optional) ---
typedef struct {
    s16 option_selected;
    // Add other menu specific data here
    // e.g., Sprite* title_sprite;
} FSMSceneData;

// Statically allocate data for this scene
static FSMSceneData fsm_scene_data;

// --- Menu Scene Lifecycle Functions ---

static void fsm_scene_init(Scene* scene_self) {
    Player_create(150, 150);
    LOGGER_DEBUG("FSM Scene: initialized");
}

static void fsm_scene_update(Scene* scene_self, fix16 dt) {
    // scene_self->data will point to menu_data
    //MenuSceneData* data = (MenuSceneData*)scene_self->data;
    if (Input_isJustPressed(JOY_1, BUTTON_A && BUTTON_B)) {
        LOGGER_DEBUG("FSM Scene: switched to main scene");
        SceneManager_setNextScene(&main_scene); // Switch to game scene
    }

    // Update menu logic, animations, etc.
}

static void fsm_scene_draw(Scene* scene_self) {
    // Draw menu elements
    // e.g., SPR_update();
    // VDP_waitVSync(); // Already handled in main loop usually by SYS_doVBlankProcess()
}

static void fsm_scene_destroy(Scene* scene_self) {
    LOGGER_DEBUG("FSM Scene: destroyed");
}

// --- FSM Scene Definition ---
Scene fsm_scene = {
    .init = fsm_scene_init,
    .update = fsm_scene_update,
    .draw = fsm_scene_draw,
    .destroy = fsm_scene_destroy,
    .data = &fsm_scene_data, // Point to our static data
    .name = "FSMScene"
};