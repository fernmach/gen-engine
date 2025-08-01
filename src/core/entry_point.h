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

#include "genengine.h"

static inline void Engine_init() {
    // Initialize game logger
    Logger_init(LOGGER_LEVEL_FATAL);

    // Initialize profiler
    Profiler_Init();

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

    // Initialize audio subsystem
    Audio_init();

    // Initialize entity subsystems
    ECS_init();

    // Initialize debug subsystems
    DBG_init();

    // Initialize scene manager subsystem
    SceneManager_init();

    // Set the initial scene
    // Note: The actual init of the scene will happen in 
    // the first SceneManager_Update() call
    SceneManager_setNextScene(&main_scene);
    //SceneManager_setNextScene(&ENGINE_MAIN_SCENE_VARIABLE);
    LOGGER_INFO("Game engine core initialized. Initial scene set.");

    // Initialize Game Specifics
    // Initialize Game Specifics - NOW HANDLED BY THE SCENE MANAGER      
    // Game.init(); // <-- DEPRECATED
    // LOGGER_INFO("Game initialized");
}

static inline void Engine_update(fix16 delta_time) {

    Profiler_StartFrame(); // START PROFILING FOR THIS FRAME
    
    // 1. Hadle Input update
    PROFILE_SCOPE(inpt_upda_id, "InputSys");
    Input_update();
    PROFILE_END_SCOPE(inpt_upda_id);

    // 2. Update Scene Manager (handles transitions & current scene's core update)
    PROFILE_SCOPE(scn_upda_id, "SceneMgrUpd");
    SceneManager_update(delta_time); // <--- CALL SCENE MANAGER UPDATE
    PROFILE_END_SCOPE(scn_upda_id);

    #if DEBUG_COLLIDER_BORDERS_ENABLED
    DebugSystem_clearColliderBoxUpdate()
    #endif

    // 3. Update  global Game Logic (Systems)
    PROFILE_SCOPE(move_upda_id, "MoveSys");
    MovementSystem_update(delta_time);
    PROFILE_END_SCOPE(move_upda_id);

    PROFILE_SCOPE(screen_upda_id, "ScreenSys");
    ScreenConstraintSystem_update();
    PROFILE_END_SCOPE(screen_upda_id);

    PROFILE_SCOPE(collis_upda_id, "ColliSys");
    CollisionSystem_update();
    PROFILE_END_SCOPE(collis_upda_id);

    #if DEBUG_COLLIDER_BORDERS_ENABLED
    DebugSystem_drawColliderBoxUpdate();
    #endif

    // PlayerControlSystem_update();
    
    PROFILE_SCOPE(audio_upda_id, "AudioSys");
    Audio_processEvents();
    PROFILE_END_SCOPE(audio_upda_id);

    PROFILE_SCOPE(fsm_upda_id, "FSMSys");
    FSMSystem_update();
    PROFILE_END_SCOPE(fsm_upda_id);

    // 4. Draw Scene
    PROFILE_SCOPE(scn_draw_id, "SceneMgrDraw");
    SceneManager_draw(); // <--- CALL SCENE MANAGER DRAW
    PROFILE_END_SCOPE(scn_draw_id);

    // 5. Draw/Render
    // SGDK handles sprite list updates, but you might have 
    // VDP background updates etc.
    PROFILE_SCOPE(render_upda_id, "RenderSys");
    RenderSystem_update();
    PROFILE_END_SCOPE(render_upda_id);
    
    #if (PROFILER_FPS_CPU_ENABLED)
    // 3.1 Optionl display debug features 
    // Show on screen CUP load if configured
    VDP_drawText("CPU:", 1, 0);
    VDP_showCPULoad(5, 0);

    // // Show FPS count on screen if configured
    VDP_drawText("FPS:", 1, 1);
    VDP_showFPS(false, 5, 1);
    #endif

    #if (PROFILER_AUDIO_CPU_LOAD_ENEBLED)        
    VDP_drawText("Audio:", 1, 2);
    Audio_showCPULoad(7, 2);    
    #endif

    // Finish profiling for the frame
    Profiler_EndFrame();

    // Draw profiler stats (usually at the end, overlayed)
    // You might want a button to toggle this
    // if (showProfiler) { // showProfiler is a bool you control via input
    Profiler_DrawStats(1, 3);
    // }

    // 4. SGDK Specific Updates
    SPR_update();            // Send sprite list to VDP
    SYS_doVBlankProcess();   // Wait for VBlank, handle DMA, tasks
}

// TODO: implement MEM_Free routines for sprites and backgrounds;
static inline void Engine_shutdown() {    
    // Potentially destroy current scene if one is active
    SceneManager_shutdown();
    Input_shutdown();
    Event_shutdown();
    //Memory_shutdown();    
    Audio_shutdown();
    Logger_shutdown();

    // End the sprite engine
    SPR_end();

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