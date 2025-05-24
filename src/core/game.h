
/**
 *  \file game.h
 *  \brief Game implementation
 *  \author Fernando Machado
 *  \date 05/2025
 *
 * Provides methods for implementing a game using the game engine.
 * These are the methos to use when implementing a game.
 */

#ifndef _GAME_H_
#define _GAME_H_

// Forward declaration for SGDK types
#include <genesis.h>

// Game engine library imports
#include "logger.h"
#include "asserts.h"

extern void Game_init();
extern void Game_update(fix16 dt);
extern void Game_draw();

#endif // GAME_H