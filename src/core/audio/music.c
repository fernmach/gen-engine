#include "music.h"

// This will point to the currently active driver.
// It's static so it's private to this file.
static const MusicDriver *s_current_driver = NULL;

// --- Facade Functions --- 
// Inicialize driver and calls the init function.
void Music_init(const MusicDriver *driver) {
    s_current_driver = driver;
    // Optionally, initialize the driver as soon as it's set
    if (s_current_driver && s_current_driver->init) {
        s_current_driver->init();
    }
}

void Music_destroy() {
    if (s_current_driver && s_current_driver->destroy) {
        s_current_driver->destroy();
    }
}

void Music_play(const u8* song) {
    if (s_current_driver && s_current_driver->play) {
        s_current_driver->play(song);
    }
}

void Music_stop() {
    if (s_current_driver && s_current_driver->stop) {
        s_current_driver->stop();
    }
}

void Music_pause() {
    if (s_current_driver && s_current_driver->pause) {
        s_current_driver->pause();
    }
}

void Music_resume() {
    if (s_current_driver && s_current_driver->resume) {
        s_current_driver->resume();
    }
}

void Music_setLoop(const s8 value) {
    if (s_current_driver && s_current_driver->setLoop) {
        s_current_driver->setLoop(value);
    }
}

void Music_setTempo(const u16 value) {
    if (s_current_driver && s_current_driver->setTempo) {
        s_current_driver->setTempo(value);
    }
}

void Music_setVolume(const u16 value) {
    if (s_current_driver && s_current_driver->setVolume) {
        s_current_driver->setVolume(value);
    }
}

bool Music_isPlaying() {
    if (s_current_driver && s_current_driver->isPlaying) {
        return s_current_driver->isPlaying();
    }
    return FALSE;
}

u16 Music_getCPULoad() {
    if (s_current_driver && s_current_driver->getCPULoad) {
        return s_current_driver->getCPULoad();
    }
    return FALSE;
}