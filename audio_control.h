#ifndef AUDIO_CONTROL_H_
#define AUDIO_CONTROL_H_

#include "MKL25Z4.h"                    // Device header

#define PTB0_Pin 0

#define TPM1_CLK_FREQ 48000000
#define TPM1_PRESCALER 7

uint8_t freq = 50;
uint8_t dutyCycle = 50;

void initPWM(void);
void setFrequency(int frequency);
void setDutyCycle(int dutyCycle);

#endif