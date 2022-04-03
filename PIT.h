#ifndef PIT_H_
#define PIT_H_

#include "mkl25z4.h"

volatile uint32_t TIMER_COUNT = 0;

void init_pit(void)
{
	TIMER_COUNT = 0;
	//Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
 //Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	//Initialize PIT0 to count down from starting_value
	// PIT->CHANNEL[0].LDVAL = 0x9FFFFF;   //every 1s 
	PIT->CHANNEL[0].LDVAL = 0x68; // every 10 us 
	//No chaining of timers
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	//Let the PIT channel generate interrupt requests
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	NVIC_SetPriority(PIT_IRQn, 1); 
	//Clear any pending IRQ from PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	//Enable the PIT interrupt in the NVIC
	NVIC_EnableIRQ(PIT_IRQn);	
}


void PIT_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	//Determine which channel triggered interrupt
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) 
	{	
		TIMER_COUNT ++;
		if (TIMER_COUNT >= 1000010)
		{
			TIMER_COUNT = 0;
		}
		//Clear interrupt request flag for channel
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	}
}

void reset_timer(void)
{
	TIMER_COUNT = 0;
}

int get_timer_duration_us(void)
{
	return TIMER_COUNT*10;
}

void delay_10_us()
{
	reset_timer();
	while(TIMER_COUNT*10 < 11) {}
}

void delay_10_ms()
{
	reset_timer();
	while(TIMER_COUNT*10 < 10000) {}
}

void delay_1_s()
{
	reset_timer();
	while(TIMER_COUNT*10 < 1000000) {}
}

#endif
