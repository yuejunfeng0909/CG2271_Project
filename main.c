#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "brain.h"
#include "UART_control.h"
#include "led_control.h"
#include "motor_control.h"
#include "audio_control.h"
#include "led_strip_control.h"
#include "Encoder.h"
#include "US_Sensor.h" 

extern osThreadId_t led_control_ID;
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initBrain();
	initLED();
	initFrontStrip();
	initRearStrip();
	initUART2();
	initMotor();
	initAudio();
	initUS_Sensor();
	initEncoder();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(brain_thread, NULL, NULL);
	led_control_ID = osThreadNew(led_control_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, NULL);
	osThreadNew(audio_control_thread, NULL, NULL);
	//osThreadNew(front_strip_control_thread, NULL, NULL);
	//osThreadNew(rear_strip_control_thread, NULL, NULL);
	osThreadNew(front_led_strip_thread, NULL, NULL);
	osThreadNew(rear_led_strip_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
