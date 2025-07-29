#include "balls.h"
#include "../core/scene.h"
#include "../core/components.h"
#include "../core/utils.h"
#include "../core/logger.h"
#include "../core/event.h"

// include our own resources
#include "res_gfx.h"
#include "res_snd.h"


// Helper initialization functions
PositionComponent MainGameScene_getRandomPosition() {    
    return (PositionComponent){
        FIX16( getRandomNumberInRange(1, 200) ), 
        FIX16( getRandomNumberInRange(1, 200) )
    };
}

VelocityComponent MainGameScene_getRandomVelocity() {    
    return (VelocityComponent){
        FIX16( getRandomNumberInRange(1, 100) ), 
        FIX16( getRandomNumberInRange(1, 100) )
    };
}

// Specific Entity Collision Handler (e.g., for a player object)
bool onBallCollision(const Event* event, void* contextData) {
    LOGGER_INFO("Event listened. Type: %u (%s), entities: %d, %d, normal: {%d,%d}, pen: %d", 
        event->type,
        Event_getDescription(event->type),
        event->data.collision.entityA,
        event->data.collision.entityB,        
        event->data.collision.normal.x,
        event->data.collision.normal.y,
        event->data.collision.penetration
    );

    // LOGGER_INFO("Even entity A/B static state: %d / %d", 
    //     g_colliders[event->data.collision.entityA].isStatic,
    //     g_colliders[event->data.collision.entityB].isStatic);

    // LOGGER_INFO("Even entity A/B triggers state: %d / %d", 
    //     g_colliders[event->data.collision.entityA].isTrigger,
    //     g_colliders[event->data.collision.entityB].isTrigger);

    return TRUE;
}

void MainGameScene_initBall() {
    Event_subscribe(EVT_COLLISION, &onBallCollision, NULL);    
}


void MainGameScene_destroyBall() {    
    Event_unsubscribe(EVT_COLLISION, &onBallCollision, NULL);
}

void MainGameScene_createBall() {
    // Creating a ball to run in the scene. And add components to it
    EntityId ball = SceneManager_createEntity();

    if (ball == ECS_MAX_ENTITIES) {
        return;
    }
    
    // Position velocity and screen constraing
    ScreenConstraintComponent constraint = {true, true}; // Corrected typo 'contraint'
    PositionComponent position = MainGameScene_getRandomPosition();
    //VelocityComponent velocity = MainGameScene_getRandomVelocity();
    VelocityComponent velocity = {FIX16(0), FIX16(0)};

    // Rigid body definition;
    ColliderComponent collider;    
    AABBColliderType box; // = { (s8)0, (s8)0, (u8)spr_donut.w, (u8)spr_donut.h };    
    
    box.x = 0;
    box.y = 0;
    box.w = spr_donut.w;;    
    box.h = spr_donut.h;

    collider.type = COLLIDER_TYPE_AABB;
    collider.shape.box = box;

    if (ball == 0) {

        collider.isStatic = FALSE;
        collider.isTrigger = FALSE;
        collider.layer = LAYER_1;
        collider.iteractWith = LAYER_2 | LAYER_3;

    } else if (ball == 1) {

        collider.isStatic = FALSE;
        collider.isTrigger = FALSE;
        collider.layer = LAYER_2;
        collider.iteractWith = LAYER_1;

    } else {

        collider.isStatic = TRUE;
        collider.isTrigger = FALSE;
        collider.layer = LAYER_3;
        collider.iteractWith = LAYER_1;

    }

    if (ball == 4) {

        collider.isStatic = TRUE;
        collider.isTrigger = TRUE;
        collider.layer = LAYER_3;
        collider.iteractWith = LAYER_1;

    }

    //LOGGER_DEBUG("Static collider %d, %d, %d", ball, collider.isStatic, collider.isTrigger);

    Entity_addComponentPosition(ball, position);
    Entity_addComponentVelocity(ball, velocity);
    Entity_addComponentCollider(ball, collider);
    //Entity_addComponentCollider1(ball, collider);
    //Entity_addComponentCollider2(ball, collider);
    Entity_addComponentScreenConstraint(ball, constraint);
    
    // Entity_addComponent(ball, COMPONENT_POSITION, &position);
    // Entity_addComponent(ball, COMPONENT_VELOCITY, &velocity);
    // Entity_addComponent(ball, COMPONENT_COLLIDER, &collider);
    // Entity_addComponent(ball, COMPONENT_SCREEN_CONSTRAINT, &constraint);    
    PAL_setPalette(PAL2, spr_donut.palette->data, DMA);
    Entity_addComponentSprite(ball, &spr_donut, PAL2); // Assuming COMPONENT_SPRITE exists
    //Entity_setComponentSpriteAnimation(ball, -1);
    Entity_setComponentSpriteVisibility(ball, HIDDEN);
}

void Ball_update(EntityId id, fix16 dx, fix16 dy) {
    // PositionComponent* position = (id, COMPONENT_POSITION);
    // LOGGER_DEBUG("Ball update %d, %d", F16_toInt(poEntity_getComponentsition->x), F16_toInt(position->y ));
    // position->x += FIX16(10);
    // position->y += FIX16(10);
    // LOGGER_DEBUG("After update %d, %d", F16_toInt(position->x), F16_toInt(position->y ));
    // Entity_setComponentValue(id, COMPONENT_POSITION, &position);
    

    //LOGGER_DEBUG("BEFORE BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));
    // g_velocities[id].dx = -g_velocities[id].dx;
    // g_velocities[id].dy = -g_velocities[id].dy;

    VelocityComponent* velocity = Entity_getComponentVelocity(id);
    velocity->dx = dx;
    velocity->dy = dy;    

    //LOGGER_DEBUG("AFTER BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));}
}

void Ball_destroy(EntityId id) {    
    Entity_destroy(0);
    //LOGGER_DEBUG("AFTER BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));}
}