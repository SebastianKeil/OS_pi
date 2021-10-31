#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	yellow_on();

	// Endless counter
	for (;;) {
		increment_counter();
		
		receive_buffer = uart_read();
		uart_write(receive_buffer);
		uart_write(10);
	}


}
