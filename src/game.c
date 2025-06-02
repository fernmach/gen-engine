#include <genesis.h> // for KDebug

#include "core/game.h"

// include our own resources
#include "res_gfx.h"
#include "res_snd.h"

// Main game events initialization
GameEvents Game = {
    .init = Game_init,
    .update = Game_update,
    .draw = Game_draw
};

void Game_init() {

    EntityId ball = Entity_create();
    PositionComponent position = {100, 100};

    char message[40];
    sprintf(message, "Entity id: %d", ball);
    VDP_drawText(message, 1, 19);
    VDP_drawText("Game initialized", 1, 20);

    Entity_addComponent(ball, COMPONENT_POSITION, &position);
    //Entity_setComponentValue(ball, COMPONENT_POSITION, &position);
    
    
    PositionComponent* anotherPos = (PositionComponent*)Entity_getComponent(ball, COMPONENT_POSITION);
    LOGGER_DEBUG("position x %d, y %d", anotherPos->x, anotherPos->y);

    // Entity_addVelocityComponent(ball, 0, 0);
    // Entity_addSpriteComponent(ball, &spr_donut, PAL0);

    // Subscribe to a test event    
    // Event_subscribe(EVT_GAME_PAUSED, &onGamePaused, "Pausing the game()");    
    // Event_subscribe(EVT_GAME_RESUMED, &onGameResume, "Resuming the game()");
    // Event_subscribe(EVT_CUSTOM, &onCustomEvent, (fix16*)FIX16(75.2));

    // Game specific initialization logic that isn't part of a generic system.
    // For example, global objects, or entities
    VDP_drawText("Game initialized", 1, 20);
}

void Game_update(fix16 dt) {
    // Game specific logic that isn't part of a generic system.
    // For example, checking for win/loss conditions, spawning new enemies, etc.

    if( Input_isPressed(JOY_1, BUTTON_A) ){
        VDP_drawText("A pressed", 1, 19);    
    }

    VDP_drawText("Game updating ", 1, 21);

    // Logger test
    // LOGGER_FATAL("FATAL log test");
    // LOGGER_ERROR("ERROR log test");
    // LOGGER_WARN("WARNING log test");
    // LOGGER_INFO("INFO log test");
    // LOGGER_DEBUG("DEBUG log test");
    // LOGGER_TRACE("TRACE log test");

    // Memory track test    
    //void* memory = MEMORY_ALLOC(1024, MEM_TAG_SYSTEM);
    //MEMORY_FREE(memory, 1024, MEM_TAG_SYSTEM);
    //MEM_alloc(1024);

    //Memory_reportUsage();
    //MEM_dump();

    //ASSERT_MSG(1==0, "ASSERT test with message");

    // // Simulate a collision for demonstration
    // bool pauseDetected = FALSE; // set this to TRUE if a collision happens
    // bool resumeDetected = FALSE; // set this to TRUE if a collision happens
    // bool stopEvents = FALSE; // set this to TRUE if a collision happens
    // static u16 frameCounter = 0;
    // frameCounter++;

    // if (frameCounter % 180 == 0) { // Every 3 seconds (at 60 FPS)
    //     pauseDetected = TRUE;
    // }

    // if (frameCounter % 360 == 0) { // Every 3 seconds (at 60 FPS)
    //     resumeDetected = TRUE;
    // }

    // if (pauseDetected && stopEvents == FALSE) {
    //     Event pauseEvent;        
    //     pauseEvent.type = EVT_GAME_PAUSED;
    //     pauseEvent.data.u8_val[0] = 10;
    //     pauseEvent.data.u8_val[1] = 20;

    //     // collisionEvent.data.collision.entityA = entity1_id;
    //     // collisionEvent.data.collision.entityB = entity2_id;
    //     // KLog_S("Physics: Publishing collision event.");
    //     Event_publish(&pauseEvent);
    // }

    //  if (resumeDetected && stopEvents == FALSE) {
    //     Event resumeEvent;        
    //     resumeEvent.type = EVT_GAME_RESUMED;
    //     resumeEvent.data.u8_val[0] = 30;
    //     resumeEvent.data.u8_val[1] = 40;

    //     // collisionEvent.data.collision.entityA = entity1_id;
    //     // collisionEvent.data.collision.entityB = entity2_id;
    //     // KLog_S("Physics: Publishing collision event.");
    //     Event_publish(&resumeEvent);
    // }

    // if (resumeDetected && stopEvents == FALSE) {
    //     Event customEvent;        
    //     customEvent.type = EVT_CUSTOM;
    //     customEvent.data.fix16_val[0] = FIX16(10.5);
    //     customEvent.data.fix16_val[1] = FIX16(60.5);

    //     // collisionEvent.data.collision.entityA = entity1_id;
    //     // collisionEvent.data.collision.entityB = entity2_id;
    //     // KLog_S("Physics: Publishing collision event.");
    //     Event_publish(&customEvent);
    // }

    // if (frameCounter % 720 == 0 && stopEvents == FALSE) { // Every 3 seconds (at 60 FPS)
    //     Event_unsubscribe(EVT_GAME_PAUSED, &onGamePaused, "Pausing the game()");
    //     Event_unsubscribe(EVT_GAME_RESUMED, &onGameResume, "Resuming the game()");
    //     Event_unsubscribe(EVT_CUSTOM, &onCustomEvent, (fix16*)FIX16(75.2));
    //     stopEvents = TRUE;
    // }
}

void Game_draw() {
    // Any game-specific drawing that isn't handled by RenderSystem.
    // e.g., VDP_drawText for score, UI elements, background layer scrolling.
    VDP_drawText("Game drawing ", 1, 22);
}


// // Specific Entity Collision Handler (e.g., for a player object)
// bool onGamePaused(const Event* event, void* contextData) {
//     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
//         event->type,
//         Event_getDescription(event->type),
//         event->data.u8_val[0],
//         event->data.u8_val[1],
//         (const char*)contextData
//     );
//     return TRUE;
// }

// // Specific Entity Collision Handler (e.g., for a player object)
// bool onGameResume(const Event* event, void* contextData) {
//     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
//         event->type,
//         Event_getDescription(event->type),
//         event->data.u8_val[0],
//         event->data.u8_val[1],
//         (const char*)contextData
//     );
//     return TRUE;
// }

// // Specific Entity Collision Handler (e.g., for a player object)
// bool onCustomEvent(const Event* event, void* contextData) {
//     LOGGER_INFO("Event fired. Type: %u (%s), values: %d, %d, context data: %s", 
//         event->type,
//         Event_getDescription(event->type),
//         event->data.fix16_val[0],
//         event->data.fix16_val[1],
//         (fix16*)contextData
//     );
//     return TRUE;
// }