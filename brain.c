#include "brain.h"
#include "self_driving.h"

extern osThreadId_t led_control_ID;
extern osSemaphoreId_t motor_sem;
extern int isFinished;
osSemaphoreId_t brain_sem;

int isSelfDriving = 0;

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
			case SELF_DRIVING_COMMAND:
				isSelfDriving = 1;
				osThreadNew(self_driving, NULL, NULL);
				break;
			case BUZZER_COMMAND:
				isFinished = 1;
				break;
			default:
				break;
		}
	}
}

