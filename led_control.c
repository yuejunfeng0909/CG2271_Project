#include "led_control.h"
#include "cmsis_os2.h"

extern uint8_t remote_command;

osThreadId_t led_control_ID;

//osSemaphoreId_t LED_sem;

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

void initLED(void)
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
	
	// Initialize LED semaphore
	//LED_sem = osSemaphoreNew(1, 0, NULL);
}

void led_control_thread(void *arguments) {
	for (;;) {
		//osSemaphoreAcquire(LED_sem, osWaitForever);
		osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
	
		switch ((remote_command >> 1) & 0b11) {
			case 0:
				// red
				led_r(remote_command & 0b1);
				break;
			case 1:
				// green
				led_g(remote_command & 0b1);
				break;
			case 2:
				// blue
				led_b(remote_command & 0b1);
				break;
		}
	}
}
