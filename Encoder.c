#include "Encoder.h"

#include "MKL25Z4.h" 
#include "motor_control.h" 
#include "cmsis_os2.h"

#define LEFT_IRT  1 // Port D Pin 1
#define RIGHT_IRT 13 // Port a Pin 13
#define MASK(x)  (1 << (x))
#define EN_STOP_DELAY  400

volatile int LEFT_COUNTER = 0;
volatile int RIGHT_COUNTER = 0;
volatile double tmp = 0;
volatile double tmp_d = 0;

int isMoving = 0;

void initEncoder(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	PORTD -> PCR[LEFT_IRT] |= (PORT_PCR_MUX(1) |   //select GPIO
														PORT_PCR_PE_MASK|
														PORT_PCR_PS_MASK|
														PORT_PCR_IRQC(0x0A)); // falling edge
	
	PORTA -> PCR[RIGHT_IRT] |= (PORT_PCR_MUX(1) |   //select GPIO
														PORT_PCR_PE_MASK|
														PORT_PCR_PS_MASK|
														PORT_PCR_IRQC(0x0A)); // falling edge
		
	 // Set PortD swithc bit to input
	PTD -> PDDR &= ~MASK(LEFT_IRT);
	PTA -> PDDR &= ~MASK(RIGHT_IRT);
 
 //Enable Interupt
 NVIC_SetPriority(PORTD_IRQn,2);
 NVIC_ClearPendingIRQ(PORTD_IRQn);
 NVIC_EnableIRQ(PORTD_IRQn);
	
	//Enable Interupt
 NVIC_SetPriority(PORTA_IRQn,2);
 NVIC_ClearPendingIRQ(PORTA_IRQn);
 NVIC_EnableIRQ(PORTA_IRQn);
}


void setMotion_and_Speed(enum directions_t Direction, uint8_t speed)
{
	switch(Direction)
	{
		case FORWARD:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, speed);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, speed);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
		case BACKWARD:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, speed);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, speed);
			break;
		case SPIN_LEFT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, speed);
			setMotorDutyCycle(RIGHT_B1, speed);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
		case SPIN_RIGHT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, speed);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, speed);
			break;
		case FORWARD_LEFT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, speed);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, speed);
			setMotorDutyCycle(RIGHT_B2, 0);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(LEFT_B1, 5);
			break;
		case FORWARD_RIGHT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, speed);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, speed);
			setMotorDutyCycle(RIGHT_B2, 0);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(RIGHT_B1, 5);
			break;
		case BACKWARD_LEFT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, speed);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, speed);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(LEFT_B2, 5);
			break;
		case BACKWARD_RIGHT:
			isMoving = 1;
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, speed);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, speed);
			osDelay(TURN_PRE_SPEEDUP_MS);
			setMotorDutyCycle(RIGHT_B2, 5);
			break;
		case STOP:
			isMoving = 0;
			setMotorDutyCycle(LEFT_B1, 0);
			setMotorDutyCycle(LEFT_B2, 0);
			setMotorDutyCycle(RIGHT_B1, 0);
			setMotorDutyCycle(RIGHT_B2, 0);
			break;
	}
}

void PORTD_IRQHandler(void) 
{
 // clear pending interrupts 
 NVIC_ClearPendingIRQ(PORTD_IRQn);
 
 if ((PORTD->ISFR & MASK(LEFT_IRT))) 
 { 
   LEFT_COUNTER++;
 }
 // clear status flags 
 PORTD->ISFR = 0xffffffff;
}

void PORTA_IRQHandler(void) 
{
 // clear pending interrupts 
 NVIC_ClearPendingIRQ(PORTA_IRQn);
 
 if ((PORTA->ISFR & MASK(RIGHT_IRT))) 
 { 
   RIGHT_COUNTER++;
 }
 // clear status flags 
 PORTA->ISFR = 0xffffffff;
}

void forward_distance(int distance)
{
	setMotion(STOP);
	osDelay(EN_STOP_DELAY);
	
	double count = distance/20.5*20;
	LEFT_COUNTER=0;
	RIGHT_COUNTER=0;
	setMotion_and_Speed(FORWARD,100);
	while(LEFT_COUNTER <= count || RIGHT_COUNTER <= count){}
	setMotion_and_Speed(BACKWARD,5);
	osDelay(200);
	setMotion(STOP);
}

void turn_degree(enum directions_t Direction,int angle)
{
	setMotion(STOP);
	osDelay(EN_STOP_DELAY);
	
	double distance = angle*17.5*3.14/360;
	double count = distance*20/20.5;
	tmp = count;
	tmp_d = distance;
	
	LEFT_COUNTER=0;
	RIGHT_COUNTER=0;

	setMotion_and_Speed(Direction,80);
	while(LEFT_COUNTER <= count || RIGHT_COUNTER <= count){}
	if(Direction == SPIN_LEFT)
	{
		setMotion_and_Speed(SPIN_RIGHT,5);
	}
	else
	{
		setMotion_and_Speed(SPIN_LEFT,5);
	}
	osDelay(200);
	setMotion(STOP);
}
