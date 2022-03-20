#include "brain.h"

extern osSemaphoreId_t LED_sem;
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
				osSemaphoreRelease(LED_sem);
				break;
			case MOTOR_COMMAND:
				osSemaphoreRelease(motor_sem);
				break;
			default:
				break;
		}
	}
}
