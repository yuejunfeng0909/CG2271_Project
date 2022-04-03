#include "led_strip_control.h" 
#include "cmsis_os2.h"

extern uint8_t remote_command;

/* Delay Function */
static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __ASM("NOP");
    nof--;
  }
}

void initFrontStrip(void)
{
  // Enable clock gating for PORTE
  SIM_SCGC5 |= (SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK);
  
  // Configure MUX settings to make all 6 pins GPIO
  PORTD->PCR[FRONT_LED_INPUT1A] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_INPUT1A] |= PORT_PCR_MUX(1);
  
  PORTD->PCR[FRONT_LED_INPUT1B] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_INPUT1B] |= PORT_PCR_MUX(1);
  
  PORTD->PCR[FRONT_LED_ENABLE1] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_ENABLE1] |= PORT_PCR_MUX(1);
  
  PORTD->PCR[FRONT_LED_INPUT2A] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_INPUT2A] |= PORT_PCR_MUX(1);
  
  PORTD->PCR[FRONT_LED_INPUT2B] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_INPUT2B] |= PORT_PCR_MUX(1);
  
  PORTD->PCR[FRONT_LED_ENABLE2] &= ~PORT_PCR_MUX_MASK; 
  PORTD->PCR[FRONT_LED_ENABLE2] |= PORT_PCR_MUX(1);
  
  // Set Data Direction Registers for PortD as Output
  PTD->PDDR |= (MASK(FRONT_LED_INPUT1A) | MASK(FRONT_LED_INPUT1B) | 
                MASK(FRONT_LED_INPUT2A) | MASK(FRONT_LED_INPUT2B) |
                MASK(FRONT_LED_ENABLE1) | MASK(FRONT_LED_ENABLE2));  
}

void front_strip_running(void) {
	// Set input 1 enable to L and input 2 enable to H
	PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE1));
	PTD->PDOR |= MASK(FRONT_LED_ENABLE2);
	// Input 1: [B] L [A] L
	PTD->PDOR &= ~((MASK(FRONT_LED_INPUT1A)) | (MASK(FRONT_LED_INPUT1B)));
	delay(RUNNING_GREEN_DELAY);
	// Input 1: [B] L [A] H
	PTD->PDOR &= ~(MASK(FRONT_LED_INPUT1B));
	PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
	delay(RUNNING_GREEN_DELAY);
	// Input 1: [B] H [A] L
	PTD->PDOR &= ~MASK(FRONT_LED_INPUT1A);
	PTD->PDOR |= MASK(FRONT_LED_INPUT1B);
	delay(RUNNING_GREEN_DELAY);
	// Input 1: [B] H [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
	delay(RUNNING_GREEN_DELAY);
	// Set input 1 enable to H and input 2 enable to L
	PTD->PDOR |= MASK(FRONT_LED_ENABLE1);
	PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE2));
	// Input 2: [B] L [A] L
	PTD->PDOR &= ~((MASK(FRONT_LED_INPUT2A)) | (MASK(FRONT_LED_INPUT2B)));
	delay(RUNNING_GREEN_DELAY);
	// Input 2: [B] L [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
	delay(RUNNING_GREEN_DELAY);
	// Input 2: [B] H [A] L
	PTD->PDOR &= ~MASK(FRONT_LED_INPUT2A);
	PTD->PDOR |= MASK(FRONT_LED_INPUT2B);
	delay(RUNNING_GREEN_DELAY);
	// Input 2: [B] H [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
	delay(RUNNING_GREEN_DELAY);
}

void front_strip_lighted_up(void) {
	 // Set input 1 enable to L and input 2 enable to H
	PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE1));
	PTD->PDOR |= MASK(FRONT_LED_ENABLE2);
	// Input 1: [B] L [A] L
	PTD->PDOR &= ~((MASK(FRONT_LED_INPUT1A)) | (MASK(FRONT_LED_INPUT1B)));
	delay(STAT_GREEN_DELAY);
	// Input 1: [B] L [A] H
	PTD->PDOR &= ~(MASK(FRONT_LED_INPUT1B));
	PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
	delay(STAT_GREEN_DELAY);
	// Input 1: [B] H [A] L
	PTD->PDOR &= ~MASK(FRONT_LED_INPUT1A);
	PTD->PDOR |= MASK(FRONT_LED_INPUT1B);
	delay(STAT_GREEN_DELAY);
	// Input 1: [B] H [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
	delay(STAT_GREEN_DELAY);
	// Set input 1 enable to H and input 2 enable to L
	PTD->PDOR |= MASK(FRONT_LED_ENABLE1);
	PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE2));
	// Input 2: [B] L [A] L
	PTD->PDOR &= ~((MASK(FRONT_LED_INPUT2A)) | (MASK(FRONT_LED_INPUT2B)));
	delay(STAT_GREEN_DELAY);
	// Input 2: [B] L [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
	delay(STAT_GREEN_DELAY);
	// Input 2: [B] H [A] L
	PTD->PDOR &= ~MASK(FRONT_LED_INPUT2A);
	PTD->PDOR |= MASK(FRONT_LED_INPUT2B);
	delay(STAT_GREEN_DELAY);
	// Input 2: [B] H [A] H
	PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
	delay(STAT_GREEN_DELAY);
}

//void front_strip_control_thread(void *arguments) {
  //for (;;) {
    //front_strip_lighted_up();
  //}
//}

void initRearStrip(void) 
{
  // Enable clock gating for PORTE
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  
  // Configure MUX settings to make all 6 pins GPIO
  PORTA->PCR[REAR_LED_POWER] &= ~PORT_PCR_MUX_MASK; 
  PORTA->PCR[REAR_LED_POWER] |= PORT_PCR_MUX(1);
  
  // Set Data Direction Registers for PortA as Output
  PTA->PDDR |= MASK(REAR_LED_POWER);
}

void rear_strip_blinks_slow(void) {
  PTA->PDOR |= MASK(REAR_LED_POWER);
  osDelay(SLOW_RED_DELAY);
  PTA->PDOR &= ~(MASK(REAR_LED_POWER));
  osDelay(SLOW_RED_DELAY);
}

void rear_strip_blinks_fast(void) {
  PTA->PDOR |= MASK(REAR_LED_POWER);
  osDelay(FAST_RED_DELAY);
  PTA->PDOR &= ~(MASK(REAR_LED_POWER));
  osDelay(FAST_RED_DELAY);
}

//void rear_strip_control_thread(void *arguments) {
  //for(;;) {
    //rear_strip_blinks_fast();
  //}
//}

void front_led_strip_thread(void *arguments)
{
	for(;;)
	{
		switch(remote_command & 0b00001111)
		{
			case 0b1100:
			case 0b1000:
			case 0b0011:
			case 0b0010:
			case 0b1111:
			case 0b1110:
			case 0b1011:
			case 0b1010:
				front_strip_running();
				break;
			default:
				front_strip_lighted_up();
		}
	}
}

void rear_led_strip_thread(void *arguments)
{
	for(;;)
	{
		switch(remote_command & 0b00001111)
		{
			case 0b1100:
			case 0b1000:
			case 0b0011:
			case 0b0010:
			case 0b1111:
			case 0b1110:
			case 0b1011:
			case 0b1010:
				rear_strip_blinks_slow();
				break;
			default:
				rear_strip_blinks_fast();
		}
	}
}
