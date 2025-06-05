
/**
 *  \file game.h
 *  \brief Game implementation
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Provides methods for implementing a game using the game engine.
 * These are the methos to use when implementing a game.
 */

#ifndef _ENG_GAME_H_
#define _ENG_GAME_H_

// Forward declaration for SGDK types
#include <genesis.h>

// Game engine library imports
#include "types.h"
#include "utils.h"
#include "logger.h"
#include "asserts.h"
#include "profiler.h"
#include "memory.h"
#include "event.h"
#include "input.h"
#include "components.h"
#include "ecs.h"
#include "scene.h"

// Implemented in game.c
extern Scene main_game_scene; 

void MainGameScene_init(Scene* scene);
void MainGameScene_update(Scene* scene, fix16 dt);
void MainGameScene_draw(Scene* scene);

// Extern declaration of the main game scene
//extern Scene GameScene;

#endif // _ENG_GAME_H_