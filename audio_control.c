#include "MKL25Z4.h"                    // Device header

#define PTB0_Pin 0

#define TPM1_CLK_FREQ 48000000
#define TPM1_PRESCALER 7

uint8_t TPM1_frequency = 50;
uint8_t TPM1_dutyCycle = 50;

void initPWM(void)
{
	// enable clock for PORT B module
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK; // reset PORTB pin 0 to disable mode
	PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(3); // Set signal mux to TPM1_CH0
	
	// enable clock for TPM0 module
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK; // clear TPM0 clock source
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); // set TPM0 clock source to MCGFLLCLK clock or MCGPLLCLK/2, but prescaler should be 1 thus should be the former
	
//	TPM1_MOD = 7500; 
//	TPM1_C0V = 3750; // set duty cycle to 50%
	TPM1_MOD = (TPM1_CLK_FREQ/(TPM1_frequency * (1 << TPM1_PRESCALER))); // set TOP value for TPM0; 48000000/128/7500 = 50Hz 
	TPM1_C0V = TPM1_MOD * TPM1_dutyCycle / 100; // set duty cycle to 50%
	
	TPM1 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK)); // reset counter to disable and prescaler to 1
	// LPTPM counter increments on every LPTPM counter clock, TPM1 prescaler to 2^7 = 128
	TPM1 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(1 << TPM1_PRESCALER));
	TPM1 -> SC &= ~(TPM_SC_CPWMS_MASK); // LPTPM counter operates in up counting mode.
	
	TPM1_C0SC &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); // diable both channels
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // set mode to "Toggle Output on match"
}

void setFrequency(int frequency){
	TPM1_frequency = frequency;
	
	TPM1_MOD = (TPM1_CLK_FREQ/(TPM1_frequency * (1 << TPM1_PRESCALER))); // update TOP value for TPM0
	TPM1_C0V = TPM1_MOD * TPM1_dutyCycle / 100; // set duty cycle
}

void setDutyCycle(int dutyCycle){
	TPM1_dutyCycle = dutyCycle;
	
	TPM1_MOD = (TPM1_CLK_FREQ/(TPM1_frequency * (1 << TPM1_PRESCALER))); // update TOP value for TPM0
	TPM1_C0V = TPM1_MOD * TPM1_dutyCycle / 100; // set duty cycle
}
