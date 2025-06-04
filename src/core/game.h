
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

// Main game event definition
typedef struct {   
   PFN_onInit init;
   PFN_onUpdate update;
   PFN_onDraw draw;
} GameEvents;

extern void Game_init();
extern void Game_update(fix16 dt);
extern void Game_draw();

extern GameEvents Game;


#endif // _ENG_GAME_H_