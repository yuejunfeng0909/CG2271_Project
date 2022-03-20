#include "motor_control.h"
#include "cmsis_os2.h"

#define TPM0_CLK_FREQ 48000000
#define TPM0_PRESCALER 7

extern uint8_t remote_command;

uint8_t TPM0_frequency = 50;
uint8_t TPM0_dutyCycle = 0;

osSemaphoreId_t motor_sem;

void initMotor(void)
{
	// enable clock for PORT C module
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	PORTC -> PCR[PTC1_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC -> PCR[PTC1_PIN] |= PORT_PCR_MUX(4);
	PORTC -> PCR[PTC2_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC -> PCR[PTC2_PIN] |= PORT_PCR_MUX(4);
	PORTC -> PCR[PTC3_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC -> PCR[PTC3_PIN] |= PORT_PCR_MUX(4);
	PORTC -> PCR[PTC4_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC -> PCR[PTC4_PIN] |= PORT_PCR_MUX(4);
	
	// enable clock for TPM0 module
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	SIM_SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK; // clear TPM0 clock source
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); // set TPM0 clock source to MCGFLLCLK clock
	
	TPM0_MOD = (TPM0_CLK_FREQ/(TPM0_frequency * (1 << TPM0_PRESCALER))); // set TOP value for TPM0; 48000000/128/7500 = 50Hz 
	TPM0_C0V = TPM0_MOD * TPM0_dutyCycle / 100; 
	TPM0_C1V = TPM0_MOD * TPM0_dutyCycle / 100; 
	TPM0_C2V = TPM0_MOD * TPM0_dutyCycle / 100; 
	TPM0_C3V = TPM0_MOD * TPM0_dutyCycle / 100; 
	
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
	
	motor_sem = osSemaphoreNew(1, 0, NULL);
}

void setMotorDutyCycle(uint8_t motor, uint8_t speed)
{
	switch(motor)
	{
		case LEFT_B2:
			LEFT_B2_SPEED = TPM0_MOD * speed / 100;
			break;
		case LEFT_B1:
			LEFT_B1_SPEED = TPM0_MOD * speed / 100;
			break;
		case RIGHT_B2:
			RIGHT_B2_SPEED = TPM0_MOD * speed / 100;
			break;
		case RIGHT_B1:
			RIGHT_B1_SPEED = TPM0_MOD * speed / 100;
			break;
	}
}

void setMotion(enum directions_t Direction)
{
	switch(Direction)
	{
		case FORWARD:
			setMotorDutyCycle(LEFT_B1, 100);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 100);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
		case BACKWARD:
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 100);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 100);
			break;
		case SPIN_LEFT:
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 100);
			setMotorDutyCycle(RIGHT_B1, 100);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
		case SPIN_RIGHT:
			setMotorDutyCycle(LEFT_B1, 100);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 100);
			break;
		case FORWARD_LEFT:
			setMotorDutyCycle(LEFT_B1, 100);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 100);
			setMotorDutyCycle(RIGHT_B2, 0);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(LEFT_B1, 5);
			break;
		case FORWARD_RIGHT:
			setMotorDutyCycle(LEFT_B1, 100);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 100);
			setMotorDutyCycle(RIGHT_B2, 0);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(RIGHT_B1, 5);
			break;
		case BACKWARD_LEFT:
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 100);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 100);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(LEFT_B2, 5);
			break;
		case BACKWARD_RIGHT:
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 100);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 100);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(RIGHT_B2, 5);
			break;
		case STOP:
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
	}
}

void motor_thread(void *arguments)
{
	for(;;)
	{
		osSemaphoreAcquire(motor_sem, osWaitForever);
		switch(remote_command & 0b00001111)
		{
			case 0b1100:
				setMotion(FORWARD);
				break;
			case 0b1000:
				setMotion(BACKWARD);
				break;
			case 0b0011:
				setMotion(SPIN_LEFT);
				break;
			case 0b0010:
				setMotion(SPIN_RIGHT);
				break;
			case 0b1111:
				setMotion(FORWARD_LEFT);
				break;
			case 0b1110:
				setMotion(FORWARD_RIGHT);
				break;
			case 0b1011:
				setMotion(BACKWARD_LEFT);
				break;
			case 0b1010:
				setMotion(BACKWARD_RIGHT);
				break;
			default:
				setMotion(STOP);
		}
	}
}
