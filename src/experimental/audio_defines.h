/**
 *  \file audio_defines.h
 *  \brief Audio definitions
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * Audio definition to mormalize driver method calls.
 */

 /*

#ifndef _ENG_AUDIO_DEFNINES_H_
#define _ENG_AUDIO_DEFNINES_H_

#include "config.h"
#include "genengine.h"

// Entity id definition
#if AUDIO_BUFFER_CAPACITY > 255
    typedef u16 AudioId;
#else
    typedef u8 AudioId;
#endif

#define SGDK_RESERVED_SAMPLE_ID 63

#if (AUDIO_XGM_DRIVER_VERSION == 1)

    #define SOUND_DRIVER_PLAY(song) XGM_startPlay(source)
    #define SOUND_DRIVER_STOP() XGM_stopPlay()
    #define SOUND_DRIVER_PAUSE() XGM_pausePlay()
    #define SOUND_DRIVER_RESUME() XGM_resumePlay()    
    #define SOUND_DRIVER_IS_PLAYING() XGM_isPlaying()

    #define SOUND_DRIVER_PCM_PLAY(id, sample, size, channel, priority) \
        XGM_setPCM ( id, sample, size ); \
        XGM_startPlayPCM( id, priority, channel );

    #define SOUND_DRIVER_GET_CPU_LOAD() XGM_getCPULoad()

    #define SOUND_DRIVER_CPU_LOAD_TYPE u32
    #define SOUND_DRIVER_CPU_LOAD_FORMAT "%ul"

#else

    #define AUDIO_MUSIC_DRIVER_PLAY(source) XGM2_play(source)
    #define AUDIO_MUSIC_DRIVER_STOP() XGM2_stop()
    #define AUDIO_MUSIC_DRIVER_PAUSE() XGM2_pause()
    #define AUDIO_MUSIC_DRIVER_RESUME() XGM2_resume()
    #define AUDIO_MUSIC_DRIVER_SET_LOOP(value) XGM2_setLoopNumber(value)
    #define AUDIO_MUSIC_DRIVER_SET_TEMPO(value) XGM2_setMusicTempo(value)

    #define AUDIO_MUSIC_DRIVER_SET_VOLUME(value) \
        XGM2_setFMVolume(value); \
        XGM2_setPSGVolume(value);
        
    #define AUDIO_MUSIC_DRIVER_IS_PLAYING() XGM2_isPlaying()

    #define AUDIO_PCM_DRIVER_PLAY(sample, size, channel, priority) \
        XGM2_playPCMEx(sample, size, channel, priority, FALSE, FALSE)

    #define AUDIO_PCM_DRIVER_STOP(channel) XGM2_stopPCM((SoundPCMChannel)channel)
    #define AUDIO_PCM_DRIVER_IS_PLAYING(channel_mask) XGM2_isPlayingPCM(channel_mask)

    #define AUDIO_DRIVER_GET_CPU_LOAD() XGM2_getCPULoad(true)
    #define AUDIO_DRIVER_CPU_LOAD_TYPE u16
    #define AUDIO_DRIVER_CPU_LOAD_FORMAT "%d"

#endif

#endif // _ENG_AUDIO_DEFNINES_H_

*/