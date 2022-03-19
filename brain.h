#include "MKL25Z4.h"                    // Device header
#include "UART_control.h"
#include "led_control.h"

#define MOTOR_COMMAND 0b1u
#define SPEED_COMMAND 0b10u
#define LED_COMMAND 0b11u
#define BUZZER_COMMAND 0b100u
#define SELF_DRIVING_COMMAND 0d5u

osSemaphoreId_t brain_sem;

extern osSemaphoreId_t LED_sem;

void Init_brain(void) {
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
