#ifndef BRAIN_H_
#define BRAIN_H_

#include "MKL25Z4.h"                    // Device header
#include "UART_control.h"
#include "led_control.h"

#define MOTOR_COMMAND 0b1u
#define SPEED_COMMAND 0b10u
#define LED_COMMAND 0b11u
#define BUZZER_COMMAND 0b100u
#define SELF_DRIVING_COMMAND 0b101u

extern osSemaphoreId_t LED_sem;

void initBrain(void);

void brain_thread(void *arguments);

#endif
