/**
 *  \file sfx.h
 *  \brief Sound effects
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * This unit provides wrapers arround audio sound effets music driver methods.
 * It is responsable to manage audio assets, its registration, playback and events. 
 */

#ifndef _ENG_SFX_H_
#define _ENG_SFX_H_

#include "sfx_driver.h"

#include <genesis.h>

void SFX_init(const SFXDriver driver);
void SFX_destroy();

void SFX_play(const SFXData* sfx);
void SFX_stop(SFXChannel channel);
bool SFX_isPlaying(const SFXChannelMask channel_mask);

#endif //_ENG_MUSIC_H_
