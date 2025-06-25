#include "balls.h"
#include "../core/scene.h"
#include "../core/components.h"
#include "../core/utils.h"
#include "../core/logger.h"

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
    Entity_addSpriteComponent(ball, &spr_donut, PAL0); // Assuming COMPONENT_SPRITE exists
}

void Ball_update(EntityId id, fix16 dx, fix16 dy) {
    // PositionComponent* position = Entity_getComponent(id, COMPONENT_POSITION);
    // LOGGER_DEBUG("Ball update %d, %d", F16_toInt(position->x), F16_toInt(position->y ));
    // position->x += FIX16(10);
    // position->y += FIX16(10);
    // LOGGER_DEBUG("After update %d, %d", F16_toInt(position->x), F16_toInt(position->y ));
    // Entity_setComponentValue(id, COMPONENT_POSITION, &position);
    

    //LOGGER_DEBUG("BEFORE BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));
    // g_velocities[id].dx = -g_velocities[id].dx;
    // g_velocities[id].dy = -g_velocities[id].dy;

    VelocityComponent velocity = Entity_getComponentVelocity(id);
    velocity.dx = dx;
    velocity.dy = dy;
    Entity_setComponentVelocity(id, velocity);

    //LOGGER_DEBUG("AFTER BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));}
}

void Ball_destroy(EntityId id) {    
    Entity_destroy(0);
    //LOGGER_DEBUG("AFTER BALL update %d, %d", F16_toInt(g_velocities[id].dx), F16_toInt(g_velocities[id].dy));}
}