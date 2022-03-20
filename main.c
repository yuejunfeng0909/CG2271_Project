/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "brain.h"
#include "UART_control.h"
#include "led_control.h"
#include "motor_control.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
//static void delay(volatile uint32_t nof) {
//	while(nof!=0) {
//		__ASM("NOP");
//		nof--;
//	}
//}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initBrain();
	initLED();
	initUART2();
	initMotor();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(brain_thread, NULL, NULL);
	osThreadNew(led_control_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
