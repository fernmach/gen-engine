// #include <genesis.h> // for KDebug

// #include "core/game.h"

// // Game scenes
// #include "scene_menu.h"

// // include our own resources
// #include "res_gfx.h"
// #include "res_snd.h"

// // Forward declare the scene instance 
// // so engine.h can see it via extern
// // do not remove or change this
// Scene main_game_scene;

// // --- Main Game Play Scene Specific Data (Optional) ---
// typedef struct {
//     EntityId ballsCount;
//     // Add other game-specific data that needs to persist for this scene
// } MainGameSceneData;

// // Static instance for this scene's data
// static MainGameSceneData main_scene_data_gameplay;

// // Helper initialization functions
// PositionComponent MainGameScene_getRandomPosition() {    
//     return (PositionComponent){
//         FIX16( getRandomNumberInRange(1, 200) ), 
//         FIX16( getRandomNumberInRange(1, 200) )
//     };
// }

// VelocityComponent MainGameScene_getRandomVelocity() {    
//     return (VelocityComponent){
//         FIX16( getRandomNumberInRange(1, 100) ), 
//         FIX16( getRandomNumberInRange(1, 100) )
//     };
// }

// void MainGameScene_createBall(MainGameSceneData* data) {

//     if (data->ballsCount >= 10)
//         return;

//     EntityId ball = Entity_create();
//     ScreenConstraintComponent constraint = {true, true}; // Corrected typo 'contraint'
//     PositionComponent position = MainGameScene_getRandomPosition();
//     VelocityComponent velocity = MainGameScene_getRandomVelocity();

//     Entity_addComponent(ball, COMPONENT_POSITION, &position);
//     Entity_addComponent(ball, COMPONENT_VELOCITY, &velocity);
//     Entity_addComponent(ball, COMPONENT_SCREEN_CONSTRAINT, &constraint);
//     Entity_addComponentSprite(ball, &spr_donut, PAL0); // Assuming COMPONENT_SPRITE exists

//     data->ballsCount++;
// }

// // Main Scene initialization
// void MainGameScene_init(Scene* scene) {

//     scene->data = &main_scene_data_gameplay; // Link scene's data pointer
//     MainGameSceneData* data = (MainGameSceneData*)scene->data;

//     data->ballsCount = 0; // Initialize scene-specific data

//     // Game specific initialization logic that isn't part of a generic system.
//     // Load resources specific to this game scene
//     // e.g., PAL_setPalette(PAL0, spr_donut.palette->data, DMA);
//     // XGM_startPlay(my_game_music);

//     // Your existing Game_init logic:
//     VDP_drawText("MainGameScene: init", 1, 20); // For debugging

//     LOGGER_INFO("MainGameScene: Initialized");    
// }

// void MainGameScene_update(Scene* scene, fix16 dt) {
//     MainGameSceneData* data = (MainGameSceneData*)scene->data;

//     // Your existing Game_update logic:
//     if( Input_isPressed(JOY_1, BUTTON_A) ) {
//         VDP_drawText("A pressed in Game Scene", 1, 19);
//         //Memory_reportUsage();
//     }

//     MainGameScene_createBall(data);

//     if (Input_isJustPressed(JOY_1, BUTTON_B)) {
//         LOGGER_DEBUG("Menu Scene: B pressed, switching to Game Scene");
//         SceneManager_setNextScene(&menu_scene); // Switch to game scene
//     }

//     // Example: Check for a condition to switch to a menu scene
//     // if (Input_isPressed(JOY_1, BUTTON_B)) {
//     //    SceneManager_SetNextScene(&main_menu_scene); // Assuming main_menu_scene exists
//     // }

//     VDP_drawText("MainGameScene: updating", 1, 21);
// }

// void MainGameScene_draw(Scene* scene) {
//     // Your existing Game_draw logic:
//     // Any game-specific drawing that isn't handled by RenderSystem.
//     // e.g., VDP_drawText for score, UI elements, background layer scrolling.
//     // VDP_drawText("Game drawing ", 1, 22); // Original line

//     // Example: Draw ball count using scene data
//     MainGameSceneData* data = (MainGameSceneData*)scene->data;
//     char str_balls[16];
//     sprintf(str_balls, "Balls: %d", (int)data->ballsCount); // Convert EntityId to int for display
//     VDP_drawText(str_balls, 1, 2);

//     VDP_drawText("MainGameScene: drawing ", 1, 22);
// }

// void MainGameScene_destroy(Scene* scene) {
//     MainGameSceneData* data = (MainGameSceneData*)scene->data;

//     // Clear all entitities before leaving the scene
//     ECS_clearAllEntities();

//     // Resets the ball count to 0
//     data->ballsCount = 0; // Reset scene data

//     // Reset all sprites
//     SPR_reset();

//     // Clear planes before leave
//     VDP_clearPlane(BG_A, TRUE);
//     VDP_clearPlane(BG_B, TRUE);

//     LOGGER_INFO("MainGameScene: Destroying");

//     // Clean up entities created by this scene
//     // This is CRUCIAL. You need a way to identify entities belonging to this scene.
//     // Option 1: Iterate all entities and destroy those with specific components.
//     // Option 2: Keep a list of entities created by this scene.
//     // Option 3: ECS_clearAllEntities(); // If appropriate for a full scene change
//     // For now, let's assume a simple clear might be needed for some entities or a tag.
//     // Example (pseudo-code):
//     // for each entity e created in this scene:
//     //     Entity_destroy(e);
//     // data->ballsCount = 0; // Reset scene data

//     // Unload resources specific to this scene
//     // e.g., SPR_reset(); // If sprites are only for this scene
//     // VDP_clearPlane(BG_A, TRUE); VDP_clearPlane(BG_B, TRUE);
//     // XGM_stopPlay();
//     // PAL_fadeOutAll(15, FALSE);
// }

// // --- Scene Definition ---
// Scene main_game_scene = {
//     .init = MainGameScene_init,
//     .update = MainGameScene_update,
//     .draw = MainGameScene_draw,
//     .destroy = MainGameScene_destroy, // IMPORTANT to implement for resource cleanup
//     .data = NULL, // Will be set to &scene_data_gameplay in init
//     .name = "MainGameScene"
// };