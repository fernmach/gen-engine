#include "music_driver.h"

// --- Implementation-specific functions ---
// These are the actual workers. They are declared 'static' to
// keep them private to this file and avoid name clashes.
static inline void XGM2_MusicDriver_setVolume(const u16	value) {
    XGM2_setFMVolume(value);
    XGM2_setPSGVolume(value);
}

static inline void XGM2_MusicDriver_destroy() {
    if(XGM2_isPlaying()) {
        XGM2_stop();
    }
}

static inline u16 XGM2_MusicDriver_getCPULoad() {
    return XGM2_getCPULoad(true);
}

const MusicDriver XGM2_MusicDriver = {
    .name = "XGM2 Driver",
    .init = NULL,
    .play = XGM2_play,
    .stop = XGM2_stop,
    .pause = XGM2_pause,
    .resume = XGM2_resume,
    .setLoop = XGM2_setLoopNumber,
    .setTempo = XGM2_setMusicTempo,
    .setVolume = XGM2_MusicDriver_setVolume,
    .isPlaying = XGM2_isPlaying,
    .destroy = XGM2_MusicDriver_destroy,
    .getCPULoad = XGM2_MusicDriver_getCPULoad
};