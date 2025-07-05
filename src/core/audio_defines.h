/**
 *  \file audio_defines.h
 *  \brief Audio definitions
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * Audio definition to mormalize driver method calls.
 */

#ifndef _ENG_AUDIO_DEFNINES_H_
#define _ENG_AUDIO_DEFNINES_H_

#include "config.h"
#include "genengine.h"

// Define audio recource id
#if AUDIO_BUFFER_CAPACITY > 255
    typedef u16 AudioId;
#else
    typedef u8 AudioId;
#endif

#define SGDK_RESERVED_SAMPLE_ID 63

#if (AUDIO_XGM_DRIVER_VERSION == 1)

    #define XGM_MUSIC_DRIVER_PLAY(song) XGM_startPlay(source)
    #define XGM_MUSIC_DRIVER_STOP() XGM_stopPlay()
    #define XGM_MUSIC_DRIVER_PAUSE() XGM_pausePlay()
    #define XGM_MUSIC_DRIVER_RESUME() XGM_resumePlay()    
    #define XGM_MUSIC_DRIVER_IS_PLAYING() XGM_isPlaying()

    #define XGM_PCM_DRIVER_PLAY(id, sample, size, channel, priority) \
        XGM_setPCM ( id, sample, size ); \
        XGM_startPlayPCM( id, priority, channel );

    #define AUDIO_XGM_DRIVER_GET_CPU_LOAD() XGM_getCPULoad()

    #define AUDIO_XGM_DRIVER_CPU_LOAD_TYPE u32
    #define AUDIO_XGM_DRIVER_CPU_LOAD_FORMAT "%ul"

#else

    #define XGM_MUSIC_DRIVER_PLAY(source) XGM2_play(source)
    #define XGM_MUSIC_DRIVER_STOP() XGM2_stop()
    #define XGM_MUSIC_DRIVER_PAUSE() XGM2_pause()
    #define XGM_MUSIC_DRIVER_RESUME() XGM2_resume()

    #define XGM_MUSIC_DRIVER_TEMPO() XGM2_setMusicTempo()
    #define XGM_MUSIC_DRIVER_LOOP() XGM2_setLoopNumber()
    #define XGM_MUSIC_DRIVER_VOLUME() XGM2_setFMVolume()
        
    #define XGM_MUSIC_DRIVER_IS_PLAYING() XGM2_isPlaying()

    // bool XGM2_playPCM	(	const u8 *	sample,
    // const u32	len,
    // const SoundPCMChannel	channel )
    #define XGM_PCM_DRIVER_PLAY(id, sample, size, channel, priority) \
        XGM2_playPCM(sample, size, channel);

    #define AUDIO_XGM_DRIVER_GET_CPU_LOAD XGM2_getCPULoad(true)

    #define AUDIO_XGM_DRIVER_CPU_LOAD_TYPE u16
    #define AUDIO_XGM_DRIVER_CPU_LOAD_FORMAT "%d"

#endif

#endif // _ENG_AUDIO_DEFNINES_H_