/*
#ifndef _ENG_MUSIC_H_
#define _ENG_MUSIC_H_

// #include "config.h"
// #include "audio_types.h"
// #include "audio_defines.h"

#include <genesis.h>

static inline void Music_play(const Music* song) {
    // AUDIO_MUSIC_DRIVER_SET_LOOP(song->properties.loop);    
    // AUDIO_MUSIC_DRIVER_SET_TEMPO(song->properties.tempo);    
    // AUDIO_MUSIC_DRIVER_SET_VOLUME(song->properties.volume);

    AUDIO_MUSIC_DRIVER_PLAY(song->audio.data);
}

static inline void Music_pause() {
    AUDIO_MUSIC_DRIVER_PAUSE();
}

static inline void Music_resume() {
    AUDIO_MUSIC_DRIVER_RESUME();
}

static inline void Music_stop() {
    AUDIO_MUSIC_DRIVER_STOP();
}

static inline void Music_setLoop(const s8 value) {
    AUDIO_MUSIC_DRIVER_SET_LOOP(value);
}

static inline void Music_setTempo(const u16 value) {
    AUDIO_MUSIC_DRIVER_SET_TEMPO(value);
}

static inline void Music_setVolume(const u16 value) {
    AUDIO_MUSIC_DRIVER_SET_VOLUME(value);
}

static inline bool Music_isPlaying() {
    return AUDIO_MUSIC_DRIVER_IS_PLAYING();
}


#endif // _ENG_MUSIC_H_
*/