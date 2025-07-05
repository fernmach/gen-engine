/**
 *  \file music.h
 *  \brief Background audio music
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * This unit provides wrapers arround audio background music driver methods.
 * It is responsable to manage background audio assets, its registration, playback and events.
 * No imediate play back is executed. The audio and it's property is published to an audio 
 * event queue, and consumed and executed later by the audio syssystem 
 */

#ifndef _ENG_MUSIC_H_
#define _ENG_MUSIC_H_

#include "music_driver.h"
#include <genesis.h>

void Music_init(const MusicDriver* driver);
void Music_destroy();

void Music_play(const u8* song);
void Music_pause();
void Music_resume();
void Music_stop();
void Music_setLoop(const s8 value);
void Music_setTempo(const u16 value);
void Music_setVolume(const u16 value);
bool Music_isPlaying();
u16 Music_getCPULoad();

#endif //_ENG_MUSIC_H_
