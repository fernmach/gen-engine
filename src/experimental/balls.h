
// input.h
#ifndef _BALLS_H_
#define _BALLS_H_

#include "../core/components.h"
#include "../core/utils.h"

// Helper initialization functions
PositionComponent MainGameScene_getRandomPosition();
VelocityComponent MainGameScene_getRandomVelocity();
void MainGameScene_createBall();
void Ball_update(EntityId id);

#   endif // _BALLS_H_