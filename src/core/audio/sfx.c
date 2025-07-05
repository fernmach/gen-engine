#include "../logger.h"
#include "../asserts.h"
#include "../defines.h"
#include "sfx.h"

// A global driver configuration
static SFXDriver s_current_sfx_driver = NULL;

void SFX_init(const SFXDriver driver) {
    ASSERT_MSG(driver == SFX_DRIVER_NONE || driver >= SFX_DRIVER_MAX,
            "SFX: SFX_init requires a valid driver");
    s_current_sfx_driver = driver;    
}

void SFX_destroy() {
    // Stop all chanels
    for(SFXChannel i=0; i < SFX_CHANNEL_MAX; i++) {    
        SFX_stop(i);
    }

    // Clear curren driver info;
    s_current_sfx_driver = SFX_DRIVER_NONE;
}

void SFX_play(const SFXData* sfx) {    
    ASSERT_MSG(!sfx, "SFX: SFX_play requires data");

    // Fast internal dispatch based on the sound type
    switch (s_current_sfx_driver) {

        case SFX_XGM2_DRIVER:
            XGM2_playPCMEx(sfx->sample, sfx->len, sfx->channel, sfx->priority, FALSE, FALSE);
            break;

        case SFX_XGM_DRIVER:
            XGM_startPlayPCM(0, sfx->priority, sfx->channel);
            break;

        default:
            LOGGER_FATAL("SFX: Unsuported driver provided on %s", FUNCTION_NAME);
            break;
    }
}

void SFX_stop(SFXChannel channel) {    
    ASSERT_MSG(!channel, "SFX: SFX_stop requires a channel");

    // Fast internal dispatch based on the sound type
    switch (s_current_sfx_driver) {

        case SFX_XGM2_DRIVER:
            XGM2_stopPCM((SoundPCMChannel)channel);
            break;

        case SFX_XGM_DRIVER:
            XGM_stopPlayPCM((SoundPCMChannel)channel);
            break;

        default:
            LOGGER_FATAL("SFX: Unsuported driver provided on %s", FUNCTION_NAME);
            break;
    }
}

bool SFX_isPlaying(const SFXChannelMask channel_mask) {    

    // Fast internal dispatch based on the sound type
    switch (s_current_sfx_driver) {

        case SFX_XGM2_DRIVER:
            return XGM2_isPlayingPCM(channel_mask);
            break;

        case SFX_XGM_DRIVER:
            return XGM_isPlayingPCM(channel_mask);
            break;

        default:
            LOGGER_FATAL("SFX: Unsuported driver provided on %s", FUNCTION_NAME);
            break;
    }

    return FALSE;
}