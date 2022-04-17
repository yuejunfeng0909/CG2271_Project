#include "US_Sensor.h"

#include "MKL25Z4.h" 
#include "PIT.h" 
#include "led_control.h"

#define TRIG_PIN  12 // Port C Pin 12
#define ECHO_PIN  13 // Port C Pin 13
#define MASK(x)  (1 << (x))

volatile int us_dis = 0;

void initUS_Sensor(void)
{
	init_pit();
	
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	
	PORTC->PCR[TRIG_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC->PCR[TRIG_PIN] |= PORT_PCR_MUX(1);
	
	PORTC->PCR[ECHO_PIN] &= ~PORT_PCR_MUX_MASK; 
	PORTC->PCR[ECHO_PIN] |= (PORT_PCR_MUX(1));
	
	// Set Data Direction Registers for PortB and PortD 
	PTC->PDDR |= MASK(TRIG_PIN);
	
	PTC -> PDDR &= ~MASK(ECHO_PIN);
}

void generate_10us_impulse(void)
{
	PTC->PDOR &= ~(MASK(TRIG_PIN)); 
	delay_10_us();
	PTC->PDOR |= MASK(TRIG_PIN); 
	delay_10_us();
	PTC->PDOR &= ~(MASK(TRIG_PIN)); 
}
void wait_until_echo(void)
{
	// no info received 
	while(!(PTC->PDIR & MASK(ECHO_PIN))) 
	{
		if(get_timer_duration_us() > 500000)
		{
			return ;
		}
	}
}	

void wait_until_echo_end(void)
{
	// info received 
	while((PTC->PDIR & MASK(ECHO_PIN))) 
	{
		if(get_timer_duration_us() > 500000)
		{
			return ;
		}
		
	}
}

int measure_distance_cm(void)
{
	generate_10us_impulse();
	reset_timer();
	wait_until_echo();
	reset_timer();
	wait_until_echo_end();
	int distance = get_timer_duration_us()/29/2;
	if(distance <= 2 || distance >= 400)
	{
		distance = -1;
	}
	delay_10_ms();
	us_dis = distance;
	return distance;
}
