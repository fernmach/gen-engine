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

    #define SOUND_DRIVER_PLAY(source) XGM2_play(source)
    #define SOUND_DRIVER_STOP() XGM2_stop()
    #define SOUND_DRIVER_PAUSE() XGM2_pause()
    #define SOUND_DRIVER_RESUME() XGM2_resume()    
    #define SOUND_DRIVER_IS_PLAYING() XGM2_isPlaying()

    // bool XGM2_playPCM	(	const u8 *	sample,
    // const u32	len,
    // const SoundPCMChannel	channel )
    #define SOUND_DRIVER_PCM_PLAY(id, sample, size, channel, priority) \
        XGM2_playPCM(sample, size, channel);

    #define SOUND_DRIVER_GET_CPU_LOAD XGM2_getCPULoad(true)

    #define SOUND_DRIVER_CPU_LOAD_TYPE u16
    #define SOUND_DRIVER_CPU_LOAD_FORMAT "%d"

#endif

#endif // _ENG_AUDIO_DEFNINES_H_