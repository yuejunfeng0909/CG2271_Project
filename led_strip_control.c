#include "led_strip_control.h" 
#include "cmsis_os2.h"

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
  for (;;) {
    // Set input 1 enable to L and input 2 enable to H
    PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE1));
    PTD->PDOR |= MASK(FRONT_LED_ENABLE2);
    // Input 1: [B] L [A] L
    PTD->PDOR &= ~((MASK(FRONT_LED_INPUT1A)) | (MASK(FRONT_LED_INPUT1B)));
    delay(50000);
    // Input 1: [B] L [A] H
    PTD->PDOR &= ~(MASK(FRONT_LED_INPUT1B));
    PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
    delay(50000);
    // Input 1: [B] H [A] L
    PTD->PDOR &= ~MASK(FRONT_LED_INPUT1A);
    PTD->PDOR |= MASK(FRONT_LED_INPUT1B);
    delay(50000);
    // Input 1: [B] H [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
    delay(50000);
    // Set input 1 enable to H and input 2 enable to L
    PTD->PDOR |= MASK(FRONT_LED_ENABLE1);
    PTD->PDOR &= ~(MASK(FRONT_LED_ENABLE2));
    // Input 2: [B] L [A] L
    PTD->PDOR &= ~((MASK(FRONT_LED_INPUT2A)) | (MASK(FRONT_LED_INPUT2B)));
    delay(50000);
    // Input 2: [B] L [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
    delay(50000);
    // Input 2: [B] H [A] L
    PTD->PDOR &= ~MASK(FRONT_LED_INPUT2A);
    PTD->PDOR |= MASK(FRONT_LED_INPUT2B);
    delay(50000);
    // Input 2: [B] H [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
    delay(50000);
  }
}

void front_strip_lighted_up(void) {
  // Set input 1 enable to L and input 2 enable to L
  PTD->PDOR |= (MASK(FRONT_LED_ENABLE2) | MASK(FRONT_LED_ENABLE2));
  for (;;) {
    // Input 1: [B] L [A] L
    PTD->PDOR &= ~((MASK(FRONT_LED_INPUT1A)) | (MASK(FRONT_LED_INPUT1B)));
    // Input 1: [B] L [A] H
    PTD->PDOR &= ~(MASK(FRONT_LED_INPUT1B));
    PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
    // Input 1: [B] H [A] L
    PTD->PDOR &= ~MASK(FRONT_LED_INPUT1A);
    PTD->PDOR |= MASK(FRONT_LED_INPUT1B);
    // Input 1: [B] H [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT1A);
    // Input 2: [B] L [A] L
    PTD->PDOR &= ~((MASK(FRONT_LED_INPUT2A)) | (MASK(FRONT_LED_INPUT2B)));
    // Input 2: [B] L [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
    // Input 2: [B] H [A] L
    PTD->PDOR &= ~MASK(FRONT_LED_INPUT2A);
    PTD->PDOR |= MASK(FRONT_LED_INPUT2B);
    // Input 2: [B] H [A] H
    PTD->PDOR |= MASK(FRONT_LED_INPUT2A);
  }
}

void front_strip_control_thread(void *arguments) {
  for (;;) {
    front_strip_running();
  }
}

void initRearStrip(void) 
{
  // Enable clock gating for PORTE
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
  
  // Configure MUX settings to make all 6 pins GPIO
  PORTA->PCR[REAR_LED_POWER] &= ~PORT_PCR_MUX_MASK; 
  PORTA->PCR[REAR_LED_POWER] |= PORT_PCR_MUX(1);
  
  // Set Data Direction Registers for PortA as Output
  PTA->PDDR |= MASK(REAR_LED_POWER);
  
  //PTA->PDOR |= MASK(REAR_LED_POWER);
}

void rear_strip_blinks_slow(void) {
  PTA->PDOR |= MASK(REAR_LED_POWER);
  osDelay(500);
  PTA->PDOR &= ~(MASK(REAR_LED_POWER));
  osDelay(500);
}

void rear_strip_blinks_fast(void) {
  PTA->PDOR |= MASK(REAR_LED_POWER);
  osDelay(250);
  PTA->PDOR &= ~(MASK(REAR_LED_POWER));
  osDelay(250);
}

void rear_strip_control_thread(void *arguments) {
  for(;;) {
    rear_strip_blinks_fast();
  }
}