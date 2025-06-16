#ifndef _ENG_SCENE_H
#define _ENG_SCENE_H

#include <genesis.h>
#include "ecs.h"

// Forward declaration of the Scene struct
typedef struct Scene Scene;

// --- Event Handler Function Pointer ---
typedef void (*PFN_onSceneInit)(Scene* scene);      // Called when scene starts
typedef void (*PFN_onSceneUpdate)(Scene* scene, fix16 delta_time);    // Called every frame for logic
typedef void (*PFN_onSceneDraw)(Scene* scene);      // Called every frame for drawing
typedef void (*PFN_onSceneDestroy)(Scene* scene);   // Called when scene ends


// The Scene structure
struct Scene {
    PFN_onSceneInit init;
    PFN_onSceneUpdate update;
    PFN_onSceneDraw draw;
    PFN_onSceneDestroy destroy;    
    void* data; // Optional: pointer to scene-specific data struct
    const char* name; // For debugging or identification    
};

extern bool g_scene_active_entities[ECS_MAX_ENTITIES];

// --- Scene Manager Functions ---

// Initializes the scene manager
void SceneManager_init();

// Sets the scene to switch to on the next update cycle
// This allows the current scene to finish its current frame's update/draw
void SceneManager_setNextScene(Scene* next_scene);

// Call this once per frame in your main game loop
// Handles scene transitions and calls the current scene's update
void SceneManager_update(fix16 delta_time);

// Call this once per frame in your main game loop (usually after update)
// Calls the current scene's draw
void SceneManager_draw();

// Gets the current active scene (can be NULL)
Scene* SceneManager_getCurrentScene();

// Creates an entity(using Entity_create) and attach it to the current scene
EntityId SceneManager_createEntity();

// Destroys an entity(using Entity_destroy) and detach it from the current scene
void SceneManager_destroyEntity(EntityId entityId);

// Destroys all entityes attached the current active screens 
// (only the ones created with the SceneManager)
void SceneManager_destroyAllEntities();

// Returns the list of all entities curretly active in the scene
const bool* SceneManager_getActiveEntities();

#endif //_ENG_SCENE_H