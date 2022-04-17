#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "audio_control.h"
#include "tone.h"
#include "self_driving.h"

extern int isFinished;
int isTone1 = 0;

void initAudio(void)
{
	// Enable clock gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Configure Mode 3 for the PWM Pin configuration
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;  // TPM1_CH0
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	// Enable clock gating for TIMER1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	// Select clock for TPM Module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // MCGFLLCLK or MCGPLLCLK/2
	
	//TPM1->MOD = 7500; 50Hz
	TPM1_C0V = 375000 / 24000;  // Set Duty cycle 33%
	
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void playSong(void) {
	int (*tone)[2]; //index 0: frequency; index 1: duration
	int len = 0;
	if (isFinished == 0) {
		tone = tone1;
		isTone1 = 1;
		len = 26; //length of tone1
	} else {
		tone = tone2;
		isTone1 = 0;
		len = 29; //length of tone2
	}

	for (int i = 0; i < len; i++) {
		if (isFinished == 1 && isTone1 == 1)
		{
			break;
		}
		if (tone[i][0] == 0) {
			TPM1_C0V = 0;  // Set Duty cycle
		} else {
			TPM1->MOD = 375000 / tone[i][0];
			TPM1_C0V = 375000 / 24000;  // Set Duty cycle
		}
		osDelay(tone[i][1]);
	}
}

void audio_control_thread(void *arguments) {
	for(;;) {
		playSong();
	}
}
