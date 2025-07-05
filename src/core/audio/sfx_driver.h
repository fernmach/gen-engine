#ifndef _ENG_SFX_DRIVER_H_
#define _ENG_SFX_DRIVER_H_

#include <genesis.h>

// Pointer to an init function
// typedef void (*PFN_onDriverInit)();
// typedef void (*PFN_onDestroy)();

typedef struct {
    const u8* sample;
    const u32 len;
    u8 channel;
    u8 priority;
} SFXData;

// typedef enum {
//     SFX_XGM_DRIVER,
//     SFX_XGM2_DRIVER,
// } SFXDriverType;

typedef enum {
    SFX_DRIVER_NONE,
    SFX_XGM_DRIVER,
    SFX_XGM2_DRIVER,
    SFX_DRIVER_MAX
} SFXDriver;

typedef enum {
  SFX_CHANNEL_AUTO = -1,
  SFX_CHANNEL_1 = 0,
  SFX_CHANNEL_2,
  SFX_CHANNEL_3,
  SFX_CHANNEL_4,

  SFX_CHANNEL_MAX
} SFXChannel;

typedef enum {
  SFX_CHANNEL1_MSK = 1 << SOUND_PCM_CH1,
  SFX_CHANNEL2_MSK = 1 << SOUND_PCM_CH2,
  SFX_CHANNEL3_MSK = 1 << SOUND_PCM_CH3,
  SFX_CHANNEL4_MSK = 1 << SOUND_PCM_CH4
} SFXChannelMask;

// This struct IS the interface. It's a "contract" that any
// music implementation must fulfill by providing these functions.
// typedef struct {    
//     // Pointer to an init function
//     PFN_onDriverInit init;
//     PFN_onDestroy destroy;
//     SFXDriverType type;
// } SFXDriver;

#endif //_ENG_SFX_DRIVER_H_