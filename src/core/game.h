
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
#include "../core/types.h"
#include "logger.h"
#include "asserts.h"
#include "memory.h"
#include "event.h"
#include "input.h"

// Main game event definition
typedef struct {   
   PFN_onInit init;
   PFN_onUpdate update;
   PFN_onDraw draw;
} GameEvents;

void Game_init();
void Game_update(fix16 dt);
void Game_draw();

GameEvents Game = {
    .init = Game_init,
    .update = Game_update,
    .draw = Game_draw
};

#endif // _ENG_GAME_H_