#include "brain.h"

extern osThreadId_t led_control_ID;
extern osSemaphoreId_t motor_sem;
osSemaphoreId_t brain_sem;

void initBrain(void) {
	brain_sem = osSemaphoreNew(1, 0, NULL);
}

void brain_thread(void *arguments) {
	// decode command
	for (;;) {
		osSemaphoreAcquire(brain_sem, osWaitForever);
		switch (remote_command >> 4) 
		{
			case LED_COMMAND:
				osThreadFlagsSet(led_control_ID, 0x00000001);
				break;
			case MOTOR_COMMAND:
				osSemaphoreRelease(motor_sem);
				break;
			default:
				break;
		}
	}
}
