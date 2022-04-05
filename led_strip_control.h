#include "MKL25Z4.h"                    // Device header
#define FRONT_LED_INPUT1A    0 
#define FRONT_LED_INPUT1B    5
#define FRONT_LED_ENABLE1    3
#define FRONT_LED_INPUT2A    7
#define FRONT_LED_INPUT2B    6
#define FRONT_LED_ENABLE2    2

#define REAR_LED_POWER       16
#define MASK(x)      (1 << (x))

#define STAT_GREEN_DELAY     10
#define RUNNING_GREEN_DELAY  50000

#define SLOW_RED_DELAY       500
#define FAST_RED_DELAY       250

void initFrontStrip(void);
//void front_strip_control_thread(void *arguments);
void initRearStrip(void);
//void rear_strip_control_thread(void *arguments);
void front_led_strip_thread(void *arguments);
void rear_led_strip_thread(void *arguments);
