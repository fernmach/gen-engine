/**
 *  \file engine.h
 *  \brief Game engine
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Provides the game engine main events and engine entry point
 */

#ifndef _ENG_MAIN_H_
#define _ENG_MAIN_H_

// Forward declaration for SGDK types
#include <genesis.h>

// game engine declarations
#include "config.h"
#include "types.h"
#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"
#include "systems.h"
#include "game.h"

static inline void Engine_init() {
    // Initialize game logger
    Logger_init(LOGGER_LEVEL_FATAL);

    // Initialize memory tracking subsystem
    Memory_init();

    // SGDK Hardware Initialization        
    // Disable interrupts during VDP initialization
    SYS_disableInts();

    SPR_init(); // Initialize sprite engine (adjust as needed based on your game)        

    // Initialize engine input sybsystem.
    JOY_init();    

    // Initialize the Video Display Processor (VDP)
    VDP_init();

    // Set screen dimensions
    VDP_setScreenWidth320(); // Or VDP_setScreenWidth256();
    VDP_setScreenHeight224(); // Or VDP_setScreenHeight240(); for PAL

    // Enable interrupts again
    SYS_enableInts();

    LOGGER_INFO("SGDK hardware initialized");

     //Input subsystem initialization
    Input_init();

    // Initialize event subsystem
    Event_init();

    // Initialize Entity subsystems
    ECS_init();    

    // Initialize Game Specifics    
    //Game_init(); // Create initial entities, load resources etc.
    Game.init();
    LOGGER_INFO("Game initialized");
}

static inline void Engine_update(fix16 delta_time) {
    // 1. Hadle Input update
    Input_update();

    // 2. Update Game Logic (Systems)    
    MovementSystem_update(delta_time);
    ScreenConstraintSystem_update();
    // PlayerControlSystem_update();
    
    // Game-specific updates not covered by generic systems
    //Game_update();
    Game.update(delta_time);    

    // 3. Draw/Render
    // SGDK handles sprite list updates, but you might have 
    // VDP background updates etc.
    RenderSystem_update();
    //Game_draw(); 
    Game.draw();

    // 3.1 Optionl display debug features 
    // Show on screen CUP load if configured
    VDP_drawText("CPU:", 1, 0);
    VDP_showCPULoad(5, 0);

    // Show FPS count on screen if configured
    VDP_drawText("FPS:", 1, 1);
    VDP_showFPS(false, 5, 1);
    
    // 4. SGDK Specific Updates
    SPR_update();            // Send sprite list to VDP
    SYS_doVBlankProcess();   // Wait for VBlank, handle DMA, tasks
}

// TODO: implement MEM_Free routines for sprites and backgrounds;
static inline void Engine_shutdown() {
    // Clean up any subsystem memory        

    Input_shutdown();
    Event_shutdown();    
    //TODO: Create memory shutdown call
    //Memory_shutdown();
    Logger_shutdown();

    LOGGER_INFO("Shutting down engine");
}

// Maing game engine loop
static inline int Engine_run() {
    // engine inicialization
    Engine_init();

    // For delta time calculation
    // Returns elapsed ticks from console reset (1/300 second based) (from SGDK)
    u32 current_time = getTick(); 
    u32 prev_time = 0;

    //  Start !!!!
    while (TRUE)
    {
        // Delta time calculation     
        current_time = getTick();        
        fix16 delta_time = F16_div(FIX16(current_time - prev_time), FIX16(300));
        prev_time = current_time; // Update prev_time for the next frame

        // engine main loop update        
        Engine_update(delta_time);                
    }

    // engine shutdown procedure    
    Engine_shutdown();

    return 0;    
}

#endif // _ENG_MAIN_H_