#include "core/genengine.h"

// External scenes (for transitioning)
#include "scene_menu.h"

// Experimental ball creation for testing;
#include "./experimental/balls.h"

// include our own resources
#include "res_gfx.h"
#include "res_snd.h"

// Forward declare the scene instance 
// Uses main_scene from gen-engine( main engine entry point)

// --- Main Game Play Scene Specific Data (Optional) ---
typedef struct {
    u8 sceneId;
    u8 ballsCount;        
    char sceneCustomData[15];
    // Add other game-specific data that needs to persist for this scene
} MainSceneData;

// Static instance for this scene's data
static MainSceneData main_scene_data;

// Reference to a list of active entities in the scen(from SceneManager)
const bool* scene_active_entities;

// Main Scene initialization
void MainScene_init(Scene* scene) {

    scene->data = &main_scene_data; // Link scene's data pointer
    MainSceneData* data = (MainSceneData*)scene->data;

    data->sceneId = 1;
    strncpy(data->sceneCustomData, "Custom Value", sizeof(data->sceneCustomData));
    data->sceneCustomData[sizeof(data->sceneCustomData) - 1] = '\0';
    data->ballsCount = 0;

    // Register ball event handlers
    MainGameScene_initBall();

    // Game specific initialization logic that isn't part of a generic system.
    // Load resources specific to this game scene
    // e.g., PAL_setPalette(PAL0, spr_donut.palette->data, DMA);
    // XGM_startPlay(my_game_music);

    // Fade out scene
    //PAL_fadeInAll(PAL0, 50, FALSE);

    // Your existing Game_init logic:
    VDP_drawText("MainScene: init", 1, 20); // For debugging
    LOGGER_INFO("MainScene: Initialized");
}

void MainScene_update(Scene* scene, fix16 dt) {
    MainSceneData* data = (MainSceneData*)scene->data;

    if (data->ballsCount < 6) {
        MainGameScene_createBall();        
        data->ballsCount++;
    }
    
    if (Input_isJustPressed(JOY_1, BUTTON_C)) {
        VDP_drawText("C pressed in switching direction", 1, 18);

        // //Ball_destroy(0);

        // scene_active_entities = SceneManager_getActiveEntities();
        // for(EntityId i=0; i < ECS_MAX_ENTITIES; i++) {
        //     if (scene_active_entities[i]) {
        //         Ball_update(i);
        //     }
        // }

    }

    // Your existing Game_update logic:
    if (Input_isJustPressed(JOY_1, BUTTON_A)) {
        LOGGER_DEBUG("Menu Scene: A pressed");
        VDP_drawText("A pressed in Main Game Scene", 1, 18);
    }

    if (Input_isJustPressed(JOY_1, BUTTON_B)) {
        LOGGER_DEBUG("Menu Scene: B pressed, switching to Menu Scene");
        VDP_drawText("B pressed in Main Menu Scene", 1, 18);
        SceneManager_setNextScene(&menu_scene); // Switch to game scene
    }

    // Your existing Game_update logic:
    
    

    // Your existing Game_update logic:
    if (Input_isPressed(JOY_1, BUTTON_UP)) {
        Ball_update(0, FIX16(0), FIX16(-100));
    } else if (Input_isReleased(JOY_1, BUTTON_UP)) {
        Ball_update(0, FIX16(0), FIX16(0));
    }
    
    if (Input_isPressed(JOY_1, BUTTON_DOWN)) {        
        Ball_update(0, FIX16(0), FIX16(100));
    } else if (Input_isReleased(JOY_1, BUTTON_DOWN)) {
        Ball_update(0, FIX16(0), FIX16(0));
    }

    // Your existing Game_update logic:
    if (Input_isPressed(JOY_1, BUTTON_LEFT)) {
        Ball_update(0, FIX16(-100), FIX16(0));
    } else if (Input_isReleased(JOY_1, BUTTON_LEFT)) {
        Ball_update(0, FIX16(0), FIX16(0));
    }
    
    if (Input_isPressed(JOY_1, BUTTON_RIGHT)) {        
        Ball_update(0, FIX16(100), FIX16(0));
    } else if (Input_isReleased(JOY_1, BUTTON_RIGHT)) {
        Ball_update(0, FIX16(0), FIX16(0));
    }

    // // Your existing Game_update logic:
    // if (Input_isPressed(JOY_1, BUTTON_LEFT)) {
    //     Ball_update(0, 0, 50);
    // } else if (Input_isPressed(JOY_1, BUTTON_RIGHT)) {
    //     Ball_update(0, 0, -50);
    // }

    // if (Input_isJustPressed(JOY_1, BUTTON_C)) {
    //     LOGGER_DEBUG("Menu Scene: B pressed, switching direction");
    //     VDP_drawText("C pressed in switching direction", 1, 18);
    // }

    VDP_drawText("MainScene: updating", 1, 21);
}

void MainScene_draw(Scene* scene) {
    // Your existing Game_draw logic:
    // Any game-specific drawing that isn't handled by RenderSystem.
    // e.g., VDP_drawText for score, UI elements, background layer scrolling.
    // VDP_drawText("Game drawing ", 1, 22); // Original line

    // Example: Draw ball count using scene data
    //MainSceneData* data = (MainSceneData*)scene->data;    
    // char str_scene_message[60];
    // sprintf(str_scene_message,
    //      "Drawing Scene id: %d, data: %s", 
    //      data->sceneId, data->sceneCustomData
    // );
    
    //VDP_drawText(str_scene_message, 1, 22);
    VDP_drawText("MainScene: drawing ", 1, 22);
}

void MainScene_destroy(Scene* scene) {
    MainSceneData* data = (MainSceneData*)scene->data;

    // Clear all entitities before leaving the scene
    
    // Resets the ball count to 0
    data->sceneId = 0; // Reset scene data    
    memset(data->sceneCustomData, 0, sizeof(data->sceneCustomData));

    // Destroy entities attached to the scene
    SceneManager_destroyAllEntities();

    // Fade out scene
    //PAL_fadeOutAll(50, FALSE);

    // Reset all sprites
    SPR_reset();

    // Clear planes before leave
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);

    LOGGER_INFO("MainScene: Destroying");

    // Clean up entities created by this scene
    // This is CRUCIAL. You need a way to identify entities belonging to this scene.
    // Option 1: Iterate all entities and destroy those with specific components.
    // Option 2: Keep a list of entities created by this scene.
    // Option 3: ECS_clearAllEntities(); // If appropriate for a full scene change
    // For now, let's assume a simple clear might be needed for some entities or a tag.
    // Example (pseudo-code):
    // for each entity e created in this scene:
    //     Entity_destroy(e);
    // data->ballsCount = 0; // Reset scene data

    // Unload resources specific to this scene
    // e.g., SPR_reset(); // If sprites are only for this scene
    // VDP_clearPlane(BG_A, TRUE); VDP_clearPlane(BG_B, TRUE);
    // XGM_stopPlay();
    // PAL_fadeOutAll(15, FALSE);
}

// --- Scene Definition ---
Scene main_scene = {
    .init = MainScene_init,
    .update = MainScene_update,
    .draw = MainScene_draw,
    .destroy = MainScene_destroy, // IMPORTANT to implement for resource cleanup
    .data = NULL, // Will be set to &main_scene_data in init
    .name = "MainScene"
};