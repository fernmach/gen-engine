// #include <genesis.h> // for KDebug

// #include "core/game.h"

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
//     PositionComponent position = getRandomPosition_game();
//     VelocityComponent velocity = getRandomVelocity_game();

//     Entity_addComponent(ball, COMPONENT_POSITION, &position);
//     Entity_addComponent(ball, COMPONENT_VELOCITY, &velocity);
//     Entity_addComponent(ball, COMPONENT_SCREEN_CONSTRAINT, &constraint);
//     Entity_addSpriteComponent(ball, &spr_donut, PAL0); // Assuming COMPONENT_SPRITE exists

//     data->ballsCount++;
// }

// // Main Scene initialization
// void MainGameScene_init(Scene* scene) {

//     // Subscribe to a test event    
//     // Event_subscribe(EVT_GAME_PAUSED, &onGamePaused, "Pausing the game()");    
//     // Event_subscribe(EVT_GAME_RESUMED, &onGameResume, "Resuming the game()");
//     // Event_subscribe(EVT_CUSTOM, &onCustomEvent, (fix16*)FIX16(75.2));

//     // Game specific initialization logic that isn't part of a generic system.
//     // For example, global objects, or entities
//     //VDP_drawText("Game initialized", 1, 20);

//     scene->data = &main_scene_data_gameplay; // Link scene's data pointer
//     MainGameSceneData* data = (MainGameSceneData*)scene->data;

//     data->ballsCount = 0; // Initialize scene-specific data

//     // Your existing Game_init logic:
//     VDP_drawText("Main Game Play Scene: Init", 1, 20); // For debugging
//     // Game specific initialization logic that isn't part of a generic system.
//     // For example, global objects, or entities
//     // VDP_drawText("Game initialized", 1, 20); // Original line

//     // Load resources specific to this game scene
//     // e.g., PAL_setPalette(PAL0, spr_donut.palette->data, DMA);
//     // XGM_startPlay(my_game_music);

//     LOGGER_INFO("MainGameScene: Initialized");
    
// }

// void MainGameScene_update(fix16 dt) {
//     // Game specific logic that isn't part of a generic system.
//     // For example, checking for win/loss conditions, spawning new enemies, etc.
    
//     if( Input_isPressed(JOY_1, BUTTON_A) ) {
//         VDP_drawText("A pressed", 1, 19);
//         //Memory_reportUsage();
//     }
    
//     createBall();

//     VDP_drawText("Game updating ", 1, 21);

//     // Logger test
//     // LOGGER_FATAL("FATAL log test");
//     // LOGGER_ERROR("ERROR log test");
//     // LOGGER_WARN("WARNING log test");
//     // LOGGER_INFO("INFO log test");
//     // LOGGER_DEBUG("DEBUG log test");
//     // LOGGER_TRACE("TRACE log test");

//     // Memory track test    
//     //void* memory = MEMORY_ALLOC(1024, MEM_TAG_SYSTEM);
//     //MEMORY_FREE(memory, 1024, MEM_TAG_SYSTEM);
//     //MEM_alloc(1024);

//     //Memory_reportUsage();
//     //MEM_dump();

//     //ASSERT_MSG(1==0, "ASSERT test with message");

//     // // Simulate a collision for demonstration
//     // bool pauseDetected = FALSE; // set this to TRUE if a collision happens
//     // bool resumeDetected = FALSE; // set this to TRUE if a collision happens
//     // bool stopEvents = FALSE; // set this to TRUE if a collision happens
//     // static u16 frameCounter = 0;
//     // frameCounter++;

//     // if (frameCounter % 180 == 0) { // Every 3 seconds (at 60 FPS)
//     //     pauseDetected = TRUE;
//     // }

//     // if (frameCounter % 360 == 0) { // Every 3 seconds (at 60 FPS)
//     //     resumeDetected = TRUE;
//     // }

//     // if (pauseDetected && stopEvents == FALSE) {
//     //     Event pauseEvent;        
//     //     pauseEvent.type = EVT_GAME_PAUSED;
//     //     pauseEvent.data.u8_val[0] = 10;
//     //     pauseEvent.data.u8_val[1] = 20;

//     //     // collisionEvent.data.collision.entityA = entity1_id;
//     //     // collisionEvent.data.collision.entityB = entity2_id;
//     //     // KLog_S("Physics: Publishing collision event.");
//     //     Event_publish(&pauseEvent);
//     // }

//     //  if (resumeDetected && stopEvents == FALSE) {
//     //     Event resumeEvent;        
//     //     resumeEvent.type = EVT_GAME_RESUMED;
//     //     resumeEvent.data.u8_val[0] = 30;
//     //     resumeEvent.data.u8_val[1] = 40;

//     //     // collisionEvent.data.collision.entityA = entity1_id;
//     //     // collisionEvent.data.collision.entityB = entity2_id;
//     //     // KLog_S("Physics: Publishing collision event.");
//     //     Event_publish(&resumeEvent);
//     // }

//     // if (resumeDetected && stopEvents == FALSE) {
//     //     Event customEvent;        
//     //     customEvent.type = EVT_CUSTOM;
//     //     customEvent.data.fix16_val[0] = FIX16(10.5);
//     //     customEvent.data.fix16_val[1] = FIX16(60.5);

//     //     // collisionEvent.data.collision.entityA = entity1_id;
//     //     // collisionEvent.data.collision.entityB = entity2_id;
//     //     // KLog_S("Physics: Publishing collision event.");
//     //     Event_publish(&customEvent);
//     // }

//     // if (frameCounter % 720 == 0 && stopEvents == FALSE) { // Every 3 seconds (at 60 FPS)
//     //     Event_unsubscribe(EVT_GAME_PAUSED, &onGamePaused, "Pausing the game()");
//     //     Event_unsubscribe(EVT_GAME_RESUMED, &onGameResume, "Resuming the game()");
//     //     Event_unsubscribe(EVT_CUSTOM, &onCustomEvent, (fix16*)FIX16(75.2));
//     //     stopEvents = TRUE;
//     // }    
// }

// void MainGameScene_draw() {
//     // Any game-specific drawing that isn't handled by RenderSystem.
//     // e.g., VDP_drawText for score, UI elements, background layer scrolling.
//     VDP_drawText("Game drawing ", 1, 22);
// }


// // // Specific Entity Collision Handler (e.g., for a player object)
// // bool onGamePaused(const Event* event, void* contextData) {
// //     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
// //         event->type,
// //         Event_getDescription(event->type),
// //         event->data.u8_val[0],
// //         event->data.u8_val[1],
// //         (const char*)contextData
// //     );
// //     return TRUE;
// // }

// // // Specific Entity Collision Handler (e.g., for a player object)
// // bool onGameResume(const Event* event, void* contextData) {
// //     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
// //         event->type,
// //         Event_getDescription(event->type),
// //         event->data.u8_val[0],
// //         event->data.u8_val[1],
// //         (const char*)contextData
// //     );
// //     return TRUE;
// // }

// // // Specific Entity Collision Handler (e.g., for a player object)
// // bool onCustomEvent(const Event* event, void* contextData) {
// //     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
// //         event->type,
// //         Event_getDescription(event->type),
// //         event->data.fix16_val[0],
// //         event->data.fix16_val[1],
// //         (fix16*)contextData
// //     );
// //     return TRUE;
// // }