#ifndef _ENG_MUSIC_H_
#define _ENG_MUSIC_H_

#include "./audio/audio.h"

/*music play helpers*/
static inline void Audio_playMusic(const u8* song, u32 len) {
    AudioEvent audioEvent = {
        .data.source = {
            .sample = song,
            .len = len
        },
        .type = AUDIO_EVENT_PLAY_MUSIC,        
    };
    Audio_dispatch(audioEvent);
}

static inline void Audio_stopMusic() {
    AudioEvent audioEvent = {        
        .type = AUDIO_EVENT_STOP_MUSIC,
    };
    Audio_dispatch(audioEvent);
}

static inline void Audio_pauseMusic() {
    AudioEvent audioEvent = {        
        .type = AUDIO_EVENT_PAUSE_MUSIC,
    };
    Audio_dispatch(audioEvent);
}

static inline void Audio_resumeMusic() {
    AudioEvent audioEvent = {        
        .type = AUDIO_EVENT_RESUME_MUSIC,
    };
    Audio_dispatch(audioEvent);
}

static inline bool Audio_isPlayingMusic() {
    return AUDIO_MUSIC_DRIVER_IS_PLAYING();
}

static inline void Audio_setMusicTempo(u16 tempo) {
    AUDIO_MUSIC_DRIVER_SET_TEMPO(tempo);
}

static inline void Audio_setMusicVolume(u16 volume) {
    AUDIO_MUSIC_DRIVER_SET_VOLUME(volume);
}

static inline void Audio_setMusicLoop(s8 loop) {
    AUDIO_MUSIC_DRIVER_SET_LOOP(loop);
}

#endif // _ENG_MUSIC_H_

