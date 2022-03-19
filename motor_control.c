#include "motor_control.h"

#define TPM0_CLK_FREQ 48000000
#define TPM0_PRESCALER 7

uint8_t TPM0_frequency = 50;
uint8_t TPM0_dutyCycle = 30;

void initMotor(void)
{
	// enable clock for PORT D module
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	PORTC -> PCR[PTC1_PIN] &= ~PORT_PCR_MUX_MASK; // reset PORTD pin 0 to disable mode
	PORTC -> PCR[PTC2_PIN] &= ~PORT_PCR_MUX_MASK; // reset PORTD pin 1 to disable mode
	PORTC -> PCR[PTC3_PIN] &= ~PORT_PCR_MUX_MASK; // reset PORTD pin 2 to disable mode
	PORTC -> PCR[PTC4_PIN] &= ~PORT_PCR_MUX_MASK; // reset PORTD pin 3 to disable mode
	
	// enable clock for TPM0 module
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK; // clear TPM0 clock source
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); // set TPM0 clock source to MCGFLLCLK clock
	
	TPM0_MOD = (TPM0_CLK_FREQ/(TPM0_frequency * (1 << TPM0_PRESCALER))); // set TOP value for TPM0; 48000000/128/7500 = 50Hz 
	TPM0_C0V = TPM0_MOD * TPM0_dutyCycle / 100; // set duty cycle to 50%
	TPM0_C1V = TPM0_MOD * TPM0_dutyCycle / 100; // set duty cycle to 50%
	TPM0_C2V = TPM0_MOD * TPM0_dutyCycle / 100; // set duty cycle to 50%
	TPM0_C3V = TPM0_MOD * TPM0_dutyCycle / 100; // set duty cycle to 50%
	
	TPM0 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK)); // reset counter to disable and prescaler to 1
	// LPTPM counter increments on every LPTPM counter clock, TPM0 prescaler to 2^7 = 128
	TPM0 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TPM0_PRESCALER));
	TPM0 -> SC &= ~(TPM_SC_CPWMS_MASK); // LPTPM counter operates in up counting mode.
	
	TPM0_C0SC &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); // clear mode for channel 0
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // set mode to "Toggle Output on match"
	TPM0_C1SC &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); // clear mode for channel 1
	TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // set mode to "Toggle Output on match"
	TPM0_C2SC &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); // clear mode for channel 2
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // set mode to "Toggle Output on match"
	TPM0_C3SC &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK); // clear mode for channel 3
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // set mode to "Toggle Output on match"
}

void enableMotor(uint8_t motor)
{
	PORTC -> PCR[motor] |= PORT_PCR_MUX(4);
}

void disableMotor(uint8_t motor)
{
	PORTC -> PCR[motor] &= ~PORT_PCR_MUX_MASK;
}

void setMotion(void)
{
	enableMotor(LEFT_B1);
	enableMotor(RIGHT_B1);
}
