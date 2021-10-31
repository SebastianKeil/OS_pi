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
		uart_write(80);
		uart_write(69);
		uart_write(78);
		uart_write(73);
		uart_write(83);
		
		uart_write(10);
	}


}
