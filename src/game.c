#include <genesis.h> // for KDebug

#include "core/game.h"

// include our own resources
//#include "res_gfx.h"
//#include "res_snd.h"

void Game_init() {
    // Game specific initialization logic that isn't part of a generic system.
    // For example, global objects, or entities
    VDP_drawText("Game initialized", 1, 20);
}

void Game_update(fix16 dt) {
    // Game specific logic that isn't part of a generic system.
    // For example, checking for win/loss conditions, spawning new enemies, etc.
    VDP_drawText("Game updating ", 1, 21);

    ASSERT_MSG(1==0, "Assertion with message");
}

void Game_draw() {
    // Any game-specific drawing that isn't handled by RenderSystem.
    // e.g., VDP_drawText for score, UI elements, background layer scrolling.
    VDP_drawText("Game drawing ", 1, 22);
}

