#include "UART_control.h"
#include "led_control.h"
#include "brain.h"

extern osSemaphoreId_t brain_sem;
uint8_t remote_command = 0;

/* Init UART2 */
void initUART2(void)
{
	uint32_t divisor, bus_clock;
	
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK; // enable clock to UART2 module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable clock to PORTE module
	
	// configure PORTE for RX
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK; // clear MUX
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4); // set MUX to alt 4, which is UART2_RX
	
	UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // disable transmit and receive while setting up the module
	
	bus_clock = DEFAULT_SYSTEM_CLOCK / 2;
	divisor = bus_clock / (BAUD_RATE * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8); // set baud rate
	UART2->BDL = UART_BDL_SBR(divisor);
	
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	UART2->C2 |= UART_C2_RE_MASK; // enable receive
	
	// initialize interrupt
  NVIC_SetPriority(UART2_IRQn, 128);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);
	
	UART2->C2 |= UART_C2_RIE_MASK; // enable RX interrupt
}


/* UART2 Read data to UART2_RX_data */
void UART2_IRQHandler (void) {
	
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		remote_command = UART2->D;
	}
	
	osSemaphoreRelease(brain_sem);
}
