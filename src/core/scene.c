#include "logger.h"
#include "scene.h"

// Internal state for the scene manager
static Scene* current_scene = NULL;
static Scene* next_scene_request = NULL;

//List of all entities created in the scene
bool g_scene_active_entities[ECS_MAX_ENTITIES];

void SceneManager_init() {
    current_scene = NULL;
    next_scene_request = NULL;

    // Any other global manager initialization if needed
    memset(g_scene_active_entities, FALSE, sizeof(g_scene_active_entities));

    LOGGER_INFO("SceneManager subsystem initialized");
}

void SceneManager_setNextScene(Scene* next_scene) {
    next_scene_request = next_scene;    
}

Scene* SceneManager_getCurrentScene() {
    return current_scene;
}

void SceneManager_update(fix16 delta_time) {
    // Handle scene transition if requested
    if (next_scene_request != NULL) {
        if (current_scene != NULL && current_scene->destroy != NULL) {            
            LOGGER_INFO("SceneManager: Destroying scene: %s", current_scene->name);
            current_scene->destroy(current_scene);
        }

        current_scene = next_scene_request;
        next_scene_request = NULL; // Clear the request

        if (current_scene != NULL && current_scene->init != NULL) {
            LOGGER_INFO("SceneManager: Initializing scene: %s", current_scene->name);
            current_scene->init(current_scene);
        }
    }

    // Update the current scene if it exists
    if (current_scene != NULL && current_scene->update != NULL) {
        current_scene->update(current_scene, delta_time);
    }
}

void SceneManager_draw() {
    // Draw the current scene if it exists
    if (current_scene != NULL && current_scene->draw != NULL) {
        current_scene->draw(current_scene);
    }
}

EntityId SceneManager_createEntity() {
    if (current_scene != NULL) {
        EntityId entityId = Entity_create();
        if (entityId < ECS_MAX_ENTITIES) {
            g_scene_active_entities[entityId] = TRUE;
            LOGGER_INFO("SceneManager: Created entity: %d in scene: %s", entityId, current_scene->name);
        }
        return entityId;
    }
    return NULL;
}

void SceneManager_destroyEntity(EntityId entityId) {
    if (current_scene != NULL && g_scene_active_entities[entityId]) {
        Entity_destroy(entityId);        
        g_scene_active_entities[entityId] = FALSE;
        LOGGER_INFO("SceneManager: Destroyed entity: %d in scene: %s", entityId, current_scene->name);
    }
}

void SceneManager_destroyAllEntities() {
    LOGGER_INFO("SceneManager: Destroying all entities from scene: %s", current_scene->name);    
    for (EntityId i = 0; i < ECS_MAX_ENTITIES; ++i) {
        SceneManager_destroyEntity(i);
    }
}