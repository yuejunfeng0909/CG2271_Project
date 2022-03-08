#include "MKL25Z4.h"                    // Device header
#include "UART_control.h"
#include "led_control.h"

void brain_thread() {
	// do actions based on the current UART values
	
	switch ((remote_command >> 1) & 0b11) {
		case 0:
			// red
			led_r(remote_command & 0b1);
			break;
		case 1:
			// green
			led_g(remote_command & 0b1);
			break;
		case 2:
			// blue
			led_b(remote_command & 0b1);
			break;
	}
}
