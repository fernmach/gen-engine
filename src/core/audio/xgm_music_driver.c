#include "music_driver.h"

static inline void XGM_MusicDriver_destroy() {
    if(XGM_isPlaying()) {
        XGM_stopPlay();
    }
}

static inline u16 XGM_MusicDriver_getCPULoad() {
    return (u16)XGM_getCPULoad();
}

const MusicDriver XGM_MusicDriver = {
    .name = "XGM Driver",
    .init = NULL,
    .play = XGM_startPlay,
    .stop = XGM_stopPlay,
    .pause = XGM_pausePlay,
    .resume = XGM_resumePlay,
    .setLoop = XGM_setLoopNumber,
    .setTempo = XGM_setMusicTempo,
    .setVolume = NULL,
    .isPlaying = XGM_isPlaying,
    .destroy = XGM_MusicDriver_destroy,
    .getCPULoad = XGM_MusicDriver_getCPULoad
};