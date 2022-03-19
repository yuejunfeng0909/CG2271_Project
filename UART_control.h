#ifndef FOO_H_
#define FOO_H_

#include "MKL25Z4.h"                    // Device header
#include "cmsis_os2.h"
#include "brain.h"

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

extern uint8_t remote_command;
extern osSemaphoreId_t brain_sem;

/* Init UART2 */
void initUART2(void);

/* UART2 Read data to UART2_RX_data */
void UART2_IRQHandler(void);

#endif
