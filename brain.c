#include "brain.h"

osSemaphoreId_t brain_sem;

void initBrain(void) {
	brain_sem = osSemaphoreNew(1, 0, NULL);
}

void brain_thread(void *arguments) {
	// decode command
	for (;;) {
		osSemaphoreAcquire(brain_sem, osWaitForever);
		switch (remote_command >> 3) {
			case LED_COMMAND:
				osSemaphoreRelease(LED_sem);
				break;
		}
	}
}
