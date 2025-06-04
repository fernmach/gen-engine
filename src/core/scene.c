#include "logger.h"
#include "scene.h"

// Internal state for the scene manager
static Scene* current_scene = NULL;
static Scene* next_scene_request = NULL;

void SceneManager_init(void) {
    current_scene = NULL;
    next_scene_request = NULL;
    // Any other global manager initialization if needed

    LOGGER_INFO("SceneManager subsystem initialized");        
}

void SceneManager_setNextScene(Scene* next_scene) {
    next_scene_request = next_scene;
}

Scene* SceneManager_getCurrentScene(void) {
    return current_scene;
}

void SceneManager_Update(void) {
    // Handle scene transition if requested
    if (next_scene_request != NULL) {
        if (current_scene != NULL && current_scene->destroy != NULL) {            
            LOGGER_INFO("SceneManager: Destroying scene: ", current_scene->name);
            current_scene->destroy(current_scene);
        }

        current_scene = next_scene_request;
        next_scene_request = NULL; // Clear the request

        if (current_scene != NULL && current_scene->init != NULL) {
            LOGGER_INFO("SceneManager: Initializing scene: ", current_scene->name);
            current_scene->init(current_scene);
        }
    }

    // Update the current scene if it exists
    if (current_scene != NULL && current_scene->update != NULL) {
        current_scene->update(current_scene);
    }
}

void SceneManager_Draw(void) {
    // Draw the current scene if it exists
    if (current_scene != NULL && current_scene->draw != NULL) {
        current_scene->draw(current_scene);
    }
}