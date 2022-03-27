#include "brain.h"

#include "MKL25Z4.h"                    // Device header
#include "UART_control.h"
#include "led_control.h"

#define MOTOR_COMMAND 0b1u
#define SPEED_COMMAND 0b10u
#define LED_COMMAND 0b11u
#define BUZZER_COMMAND 0b100u
#define SELF_DRIVING_COMMAND 0b101u

osSemaphoreId_t brain_sem;

//extern osSemaphoreId_t LED_sem;
//extern osSemaphoreId_t motor_sem;
//extern osSemaphoreId_t speed_sem;
//extern osSemaphoreId_t audio_sem;
//extern osSemaphoreId_t sd_sem;


//void Init_brain(void) {
//	brain_sem = osSemaphoreNew(1, 0, NULL);
//}

//void brain_thread(void *arguments) {
//	// decode command
//	for (;;) {
//		osSemaphoreAcquire(brain_sem, osWaitForever);
//		switch (remote_command >> 3) {
//			case LED_COMMAND:
//				osSemaphoreRelease(LED_sem);
//				break;
//			case MOTOR_COMMAND:
//				osSemaphoreRelease(motor_sem);
//				break;
//			case SPEED_COMMAND:
//				osSemaphoreRelease(speed_sem);
//				break;
//			case BUZZER_COMMAND:
//				osSemaphoreRelease(audio_sem);
//				break;
//			case SELF_DRIVING_COMMAND:
//				osSemaphoreRelease(sd_sem);
//				break;
//		}
//	}
//}

