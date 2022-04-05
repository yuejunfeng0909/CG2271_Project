#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "MKL25Z4.h"                    // Device header

#define PTC1_PIN 1
#define PTC2_PIN 2
#define PTC3_PIN 3
#define PTC4_PIN 4

#define LEFT_B2 PTC1_PIN
#define LEFT_B1 PTC2_PIN
#define RIGHT_B2 PTC3_PIN
#define RIGHT_B1 PTC4_PIN

#define LEFT_B2_SPEED TPM0_C0V
#define LEFT_B1_SPEED TPM0_C1V
#define RIGHT_B2_SPEED TPM0_C2V
#define RIGHT_B1_SPEED TPM0_C3V

#define TPM0_CLK_FREQ 48000000
#define TPM0_PRESCALER 7

#define TURN_PRE_SPEEDUP_MS 100

enum directions_t
{
	FORWARD, BACKWARD, SPIN_LEFT, SPIN_RIGHT, FORWARD_LEFT, FORWARD_RIGHT, BACKWARD_LEFT, BACKWARD_RIGHT, STOP
};

void initMotor(void);
void setMotion(enum directions_t Direction);
void setMotorDutyCycle(uint8_t motor, uint8_t speed);
void motor_thread(void *arguments);

#endif
