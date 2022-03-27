#ifndef AUDIO_CONTROL_H_

#include "MKL25Z4.h"  // Device header

#define AUDIO_CONTROL_H_
                  
#define PTB0_Pin 0




void initAudio(void);
void playSong(void);
void audio_control_thread(void *arguments);

#endif

