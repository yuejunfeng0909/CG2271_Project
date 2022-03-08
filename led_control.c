#include "led_control.h"

void led_r(uint8_t val)
{
	if (val == 0)
		PTB->PDOR |= MASK(RED_LED);
	else
		PTB->PDOR &= ~MASK(RED_LED);
}

void led_g(uint8_t val)
{
	if (val == 0)
		PTB->PDOR |= MASK(GREEN_LED);
	else
		PTB->PDOR &= ~MASK(GREEN_LED);
}

void led_b(uint8_t val)
{
	if (val == 0)
		PTD->PDOR |= MASK(BLUE_LED);
	else
		PTD->PDOR &= ~MASK(BLUE_LED);
}

void led_control(enum color_t color)
{
	switch(color)
	{
		case RED:
			led_r(1);
			led_g(0);
			led_b(0);
			break;
		case GREEN:
			led_r(0);
			led_g(1);
			led_b(0);
			break;
		case BLUE:
			led_r(0);
			led_g(0);
			led_b(1);
			break;
		default:
			led_r(0);
			led_g(0);
			led_b(0);
	}
}

void InitLED(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD 
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED)); 
	PTD->PDDR |= MASK(BLUE_LED);
	
	// Initialize all led to off
	led_control(BLACK);
}
