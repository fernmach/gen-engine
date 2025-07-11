/**
 *  \file audio.h
 *  \brief Audio subsystem
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * This unit provides wrapers arround audio direver methods.
 * It is responsable to manage all audio assets, its registration, playback and events.
 * No imediate play back is executed. The audiois published to an audio event queue, and
 * consumed/executed later by the audio subsystem
 */

#ifndef _ENG_AUDIO_H_
#define _ENG_AUDIO_H_

#include "../config.h"
#include "audio_buffer.h"

// This is the control structure for our buffer.
extern AudioBuffer g_audio_event_bus;
extern AudioEvent g_audio_event_buffer_data[AUDIO_BUFFER_CAPACITY];

/*Audio music driver wrapper(for future vendor driver implementation)*/
#define AUDIO_MUSIC_DRIVER_PLAY(source) XGM2_play(source)
#define AUDIO_MUSIC_DRIVER_PAUSE() XGM2_pause()
#define AUDIO_MUSIC_DRIVER_STOP() XGM2_stop()
#define AUDIO_MUSIC_DRIVER_RESUME() XGM2_resume()
#define AUDIO_MUSIC_DRIVER_SET_TEMPO(tempo) XGM2_setMusicTempo(tempo)
#define AUDIO_MUSIC_DRIVER_SET_VOLUME(volume) \
    XGM2_setFMVolume (volume); \
    XGM2_setPSGVolume(volume);
#define AUDIO_MUSIC_DRIVER_SET_LOOP(volume) XGM2_setLoopNumber(volume);
#define AUDIO_MUSIC_DRIVER_IS_PLAYING() XGM2_isPlaying()

/*Audio sfx driver wrapper(for future vendor driver implementation)*/
#define AUDIO_PCM_DRIVER_PLAY(sample, size, channel, priority, halfRate, loop) \
        XGM2_playPCMEx(sample, size, channel, priority, halfRate, loop)
/*Audio pcm driver wrapper(for future vendor driver implementation)*/
#define AUDIO_PCM_DRIVER_STOP(channel) XGM2_stopPCM((SoundPCMChannel)channel)
#define AUDIO_PCM_DRIVER_IS_PLAYING(channel_mask) XGM2_isPlayingPCM(channel_mask)

/*Audio debug helpers*/
#define AUDIO_DRIVER_GET_CPU_LOAD() XGM2_getCPULoad(true)

void Audio_init();
void Audio_clearBuffer();
void Audio_shutdown();
//void Audio_dispatch(AudioEvent event);

static inline void Audio_dispatch(AudioEvent event) {
    if ( !AudioBuffer_write( &g_audio_event_bus, event ) ) {
        LOGGER_WARN("Audio Event: buffer full. Dropped source play %s", event.name);
    }

    //SOUND_DRIVER_PLAY(source);    
    // Try to add a "shoot" event.
    LOGGER_DEBUG("Audio buffer: Size: %d, Cap: %d, Count: %d",
        AUDIO_BUFFER_CAPACITY, g_audio_event_bus.capacity,
        g_audio_event_bus.count
    );

    LOGGER_DEBUG("Audio buffer: Event type: %d", event.type);    
}

//-- Debuging utility functions
static inline u16 Audio_getCPULoad() {
    return AUDIO_DRIVER_GET_CPU_LOAD();
}

static inline void Audio_showCPULoad(u16 x, u16 y) {
    char audio_load_indicator[4];
    sprintf(audio_load_indicator, "%d", AUDIO_DRIVER_GET_CPU_LOAD() );
    VDP_clearText(x, y, 4);
    VDP_drawText(audio_load_indicator, x, y);
}

static inline void Audio_processEvents() {
    AudioEvent event;

    // Use a 'while' loop, NOT an 'if'.
    // This ensures we process all pending events in a single frame if needed,
    // preventing a backlog.
    while (AudioBuffer_read(&g_audio_event_bus, &event)) {
        
        switch (event.type) {
            case AUDIO_EVENT_PLAY_SFX:
                AUDIO_PCM_DRIVER_PLAY(
                    event.data.source.sample,
                    event.data.source.len,
                    SOUND_PCM_CH_AUTO,
                    6,
                    FALSE,
                    FALSE
                );
                break;

            case AUDIO_EVENT_PLAY_SFX_EX:
                AUDIO_PCM_DRIVER_PLAY(
                    event.data.sfx.source.sample,
                    event.data.sfx.source.len,
                    event.data.sfx.properties.channel,
                    event.data.sfx.properties.priority,
                    event.data.sfx.properties.halfRate,
                    event.data.sfx.properties.loop
                );
                break;

            case AUDIO_EVENT_STOP_SFX:
                AUDIO_PCM_DRIVER_STOP(event.data.channel);
                break;

            case AUDIO_EVENT_PLAY_MUSIC:
                AUDIO_MUSIC_DRIVER_PLAY(event.data.source.sample);
                break;            

            case AUDIO_EVENT_PAUSE_MUSIC:
                AUDIO_MUSIC_DRIVER_PAUSE();
                break;            
                
            case AUDIO_EVENT_RESUME_MUSIC:
                AUDIO_MUSIC_DRIVER_RESUME();
                break;

            case AUDIO_EVENT_STOP_MUSIC:
                AUDIO_MUSIC_DRIVER_STOP();
                break;

            default:
                LOGGER_WARN("Audio: Failed to process event. Unknon type %d", event.type);
                break;
        }
        
    } 
}

#endif