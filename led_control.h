#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include "MKL25Z4.h"                    // Device header
#define RED_LED			18
#define GREEN_LED		19
#define BLUE_LED		1
#define MASK(x)			(1 << (x))

extern uint8_t remote_command;

enum color_t
{
	RED, GREEN, BLUE, BLACK
};

void led_r(uint8_t val);

void led_g(uint8_t val);

void led_b(uint8_t val);

void led_control(enum color_t color);

void initLED(void);

void led_control_thread(void *arguments);

#endif
