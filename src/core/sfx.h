#ifndef _ENG_SFX_H_
#define _ENG_SFX_H_

#include "./audio/audio.h"

/*music play helpers*/
static inline void Audio_playSFX(const u8* sample, u32 len) {
    AudioEvent audioEvent = {
        .data.source = {
            .sample = sample,
            .len = len
        },
        .type = AUDIO_EVENT_PLAY_SFX,
    };
    Audio_dispatch(audioEvent);
}

//TODO: Create generic channel enum t o work with future generic drivers.
static inline void Audio_playSFXEx(
        const u8* sample,
        u32 len,
    	const SoundPCMChannel channel,
        const u8 priority,
        const bool halfRate,
        const bool loop
) {
    AudioEvent audioEvent = {
        .data.sfx = {
            .source = {        
                .sample = sample,
                .len = len            
            },
            .properties = {
                .channel = channel,
                .priority = priority,
                .halfRate = halfRate,
                .loop = loop
            }
        },
        .type = AUDIO_EVENT_PLAY_SFX_EX
    };
    Audio_dispatch(audioEvent);
}

/*music play helpers*/
static inline void Audio_stopSFX(const SoundPCMChannel channel) {
    AudioEvent audioEvent = {
        .data.channel = channel,
        .type = AUDIO_EVENT_STOP_SFX,
    };
    Audio_dispatch(audioEvent);
}

/*music play helpers*/
static inline u8 Audio_isPlayingSFX(const u16 channel_mask) {   
    return AUDIO_PCM_DRIVER_IS_PLAYING(channel_mask);
}

#endif //_ENG_SFX_H_