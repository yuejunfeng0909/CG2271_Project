#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "audio_control.h"


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
	
	//TPM1->MOD = 375000 / 8000;
	//TPM1_C0V = 375000 / 8000;  // Set Duty cycle 100%
	
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void playSong(void) {
	int songFreq[7] = {1000, 2000, 3000, 4000, 5000, 6000, 7000};
	for (int i = 0; i < 7; i++) {
		TPM1->MOD = 375000 / songFreq[i];
	  TPM1_C0V = 375000 / songFreq[i];  // Set Duty cycle
		osDelay(1000);
	}
}

void audio_control_thread(void *arguments) {
	for(;;) {
		playSong();
	}
}
