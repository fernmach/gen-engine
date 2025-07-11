/**
 *  \file sfx.h
 *  \brief Sound effects
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * This unit provides wrapers arround audio sound effets music driver methods.
 * It is responsable to manage audio assets, its registration, playback and events. 
 */

 /*
#ifndef _ENG_SFX_H_
#define _ENG_SFX_H_

// #include "config.h"
// #include "audio_types.h"
// #include "audio_defines.h"

#include <genesis.h>

static inline bool SFX_play(const SFX* sfx) {
    return AUDIO_PCM_DRIVER_PLAY (
        sfx->audio.data,
        sfx->audio.len,
        sfx->properties.channel,
        sfx->properties.priority
    );
}

static inline void SFX_stop(SFXChannel channel) {
    AUDIO_PCM_DRIVER_STOP(channel);
}

static inline SFXChannelMask SFX_isPlaying(const SFXChannelMask channel_mask) {    
    return AUDIO_PCM_DRIVER_IS_PLAYING(channel_mask);
}
    
#endif //_ENG_MUSIC_H_
*/