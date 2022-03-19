#ifndef AUDIO_CONTROL_H_
#define AUDIO_CONTROL_H_

#include "MKL25Z4.h"                    // Device header

#define PTB0_Pin 0

#define C_Freq 262
#define G_Freq 392
#define A_Freq 440

void initAudio(void);
void playSong(void);
void audio_control_thread(void *arguments);

#endif
