#ifndef _ENG_SYSTEMS_H_
#define _ENG_SYSTEMS_H_

// Forward declaration for SGDK types
typedef short s16;
typedef s16 fix16;

void MovementSystem_update(fix16 dt);
void ScreenConstraintSystem_update();
//void PlayerControlSystem_update();
void RenderSystem_update();

#endif // SYSTEMS_H
