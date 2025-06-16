#include "logger.h"
#include "scene.h"

// Internal state for the scene manager
static Scene* current_scene = NULL;
static Scene* next_scene_request = NULL;

bool g_scene_active_entities[ECS_MAX_ENTITIES];

void SceneManager_init() {
    current_scene = NULL;
    next_scene_request = NULL;
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
        if (entityId <= ECS_MAX_ENTITIES) {
            LOGGER_INFO("SceneManager: Created entity: %d in scene: %s", entityId, current_scene->name);
        }
        return entityId;
    }
    return NULL;
}

void SceneManager_destroyEntity(EntityId entityId) {
    if (current_scene != NULL) {       
        Entity_destroy(entityId);
    }
}

void SceneManager_destroyAllEntities() {
    // for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
    //     if ( g_scene_active_entities[i] )
    //         SceneManager_destroyEntity(i);
    // }
    ECS_clearAllEntities();
}

const bool* SceneManager_getActiveEntities() {
    LOGGER_INFO("SceneManager: Retrieving list of active entities" );
    memset(g_scene_active_entities, FALSE, sizeof(g_scene_active_entities));

    for (EntityId i = 0; i < ACTIVE_ENTITY_COUNT; ++i) {
        if ( g_entity_component_masks[i] != COMPONENT_NONE ) {
            g_scene_active_entities[i] = TRUE;
        }
    }

    return g_scene_active_entities;
}