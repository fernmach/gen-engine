#include "fsm_player.h"
#include "res_gfx.h"

// --- Forward declarations for state functions ---
static void player_idle_onEnter(EntityId entityId);
static void player_idle_onUpdate(EntityId entityId);

static void player_walking_onEnter(EntityId entityId);
static void player_walking_onUpdate(EntityId entityId);

// static void player_jumping_onEnter(EntityId entityId);
// static void player_jumping_onUpdate(EntityId entityId);

static void player_attacking_onEnter(EntityId entityId);
static void player_attacking_onUpdate(EntityId entityId);

// --- The Player FSM Definition ---
// This array maps the PlayerState enum to the actual functions.
const State playerStates[] = {
    [PLAYER_STATE_IDLE]     = { player_idle_onEnter, player_idle_onUpdate, NULL },
    [PLAYER_STATE_WALKING]  = { player_walking_onEnter, player_walking_onUpdate, NULL },
     [PLAYER_STATE_ATTACKING] = { player_attacking_onEnter, player_attacking_onUpdate, NULL }
    // [PLAYER_STATE_JUMPING]  = { player_jumping_onEnter, player_jumping_onUpdate, NULL },
    
};

const FSMDefinition playerFSMDefinition = {
    .states = playerStates,
    .numStates = sizeof(playerStates) / sizeof(State)
};

// --- State Implementations ---

// -- IDLE
static void player_idle_onEnter(EntityId entityId) {
    // Get sprite component and change animation
    //SpriteComponent* sprite = g_ecs_get_sprite_component(entityId); // <--- Your ECS function
    // SPR_setAnim(sprite->sgdk_sprite, ANIM_IDLE); 
    VDP_clearTextLine(18);
    VDP_drawText("State: IDLE", 1, 18);
}

static void player_idle_onUpdate(EntityId entityId) {

    /// Your existing Game_update logic:
    if (Input_isPressed(JOY_1, BUTTON_RIGHT) ||
        Input_isPressed(JOY_1, BUTTON_LEFT)
    ) {
        FSM_changeState(entityId, PLAYER_STATE_WALKING);
    }
 
    if (Input_isPressed(JOY_1, BUTTON_A)) {
        FSM_changeState(entityId, PLAYER_STATE_ATTACKING);
    }    
}

// -- WALKING
static void player_walking_onEnter(EntityId entityId) {
    // SpriteComponent* sprite = g_ecs_get_sprite_component(entityId);
    // SPR_setAnim(sprite->sgdk_sprite, ANIM_WALK);
    VDP_clearTextLine(18);
    VDP_drawText("State: WALKING", 1, 18);
}

static void player_walking_onUpdate(EntityId entityId) {
    VelocityComponent* vel = Entity_getComponentVelocity(entityId); // <--- Your ECS function

    if (Input_isPressed(JOY_1, BUTTON_RIGHT)) {
        vel->dx = FIX16(100.0);
    } else if (Input_isPressed(JOY_1, BUTTON_LEFT)) {
        vel->dx = FIX16(-100.0);
    } else {
        // No movement, go back to idle
        vel->dx = FIX16(0.0);
        FSM_changeState(entityId, PLAYER_STATE_IDLE);
    }

    if (Input_isPressed(JOY_1, BUTTON_A)) {
        FSM_changeState(entityId, PLAYER_STATE_ATTACKING);
    }
}

// // -- JUMPING
// static void player_jumping_onEnter(EntityId entityId) {
//     VelocityComponent* vel = Entity_getComponentVelocityRef(entityId);
//     vel->dy = FIX16(-150); // Apply initial upward velocity
//     // Play sound
//     // XGM_startPlayPCM(SFX_JUMP, 1, SOUND_PCM_CH2);
//     VDP_drawText("State: JUMPING", 1, 1);
// }

// static void player_jumping_onUpdate(EntityId entityId) {
//     // In a real game, a physics system would handle gravity.
//     // When the player hits the ground, the physics system would
//     // call FSM_changeState(entityId, PLAYER_STATE_IDLE).
// }

// -- ATTACKING
static void player_attacking_onEnter(EntityId entityId) {
    FSMComponent* fsm = Entity_getComponentFSM(entityId);
    fsm->stateTimer = 10; // Attack lasts for 30 frames
    VDP_clearTextLine(18);
    VDP_drawText("State: ATTACKING", 1, 18);
}

static void player_attacking_onUpdate(EntityId entityId) {
    FSMComponent* fsm = Entity_getComponentFSM(entityId);
    // When the timer runs out, go back to idle
    if (fsm->stateTimer == 0) {
        FSM_changeState(entityId, PLAYER_STATE_IDLE);
    }
}

// --- Player Creation ---
EntityId Player_create(s16 x, s16 y) {
    EntityId entityId = Entity_create();

    // Add and initialize the FSM component
    FSMComponent fsm = {
        .definition = &playerFSMDefinition,
        .currentState = PLAYER_STATE_IDLE,
        .stateTimer = 0
    };
    
    // Add required components
    Entity_addComponentPosition(entityId, (PositionComponent){FIX16(x),FIX16(y)});
    Entity_addComponentVelocity(entityId, (VelocityComponent){FIX16(0),FIX16(0)});
    Entity_addComponentSprite(entityId, &spr_donut, PAL0);
    Entity_addComponentFSM(entityId, fsm);

    // Manually call the first onEnter
    if (playerFSMDefinition.states[PLAYER_STATE_IDLE].onEnter) {
        playerFSMDefinition.states[PLAYER_STATE_IDLE].onEnter(entityId);
    }
    
    return entityId;
}