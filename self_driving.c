#include "self_driving.h"
#include "MKL25Z4.h" 
#include "US_Sensor.h" 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "led_control.h"
#include "motor_control.h"
#include "Encoder.h"

osSemaphoreId_t selfDriving;

volatile int US_STOP_DISNTANCE = 20;
volatile int US_DIS = 99;
int isFinished = 0;

void approach(void)
{
	
		setMotion_and_Speed(FORWARD,50);
	
		US_DIS = measure_distance_cm();
		
		while((US_DIS> (4 * US_STOP_DISNTANCE)) || (US_DIS == -1))
		{
			US_DIS = measure_distance_cm();
		}
				
		setMotion_and_Speed(FORWARD,20);
		while((US_DIS> (3 * US_STOP_DISNTANCE)) || (US_DIS == -1)) {US_DIS = measure_distance_cm();}
				
		setMotion_and_Speed(FORWARD,10);
		while((US_DIS> (1 * US_STOP_DISNTANCE)) || (US_DIS == -1)) {US_DIS = measure_distance_cm();}
				
		setMotion(STOP);
		osDelay(500);
}

void run_around_obstacle(void)
{
	turn_degree(SPIN_RIGHT,90);
	forward_distance(50);
	turn_degree(SPIN_LEFT,90);
	forward_distance(100);
	turn_degree(SPIN_LEFT,90);
	forward_distance(100);
	turn_degree(SPIN_LEFT,90);
//	forward_distance(80);
//	turn_degree(SPIN_LEFT,90);
//	forward_distance(40);
//	turn_degree(SPIN_RIGHT,90);
}

void self_driving(void *argument)
{
	// add semaphore
	US_STOP_DISNTANCE = 20;
	US_DIS = 99;
	isFinished = 0;
	approach();
	run_around_obstacle();
	setMotion_and_Speed(FORWARD,100);
	osDelay(3000);
	setMotion(STOP);
	isFinished = 1;
}
