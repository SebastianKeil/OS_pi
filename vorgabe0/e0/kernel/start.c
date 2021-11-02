#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	yellow_on();
	uart_write(10);
	kprintf("zum beenden: erst str+a, dann x");
	uart_write(10);
	kprintf("Hallo ich bin Dr. Kernel, gib eine Zahl ein: ");
	uart_write(10);
	

	// Endless counter
	for (;;) {
		increment_counter();
		
		if(counter == 3){
			kprintf("Da kommt nichts neues!");
			uart_write(10);
			
		}
		//c, s, x, i, u, p
		//receive_buffer = uart_read();
		int i = uart_read() - 48;
		int n = 9;
		char str[] = "jaschas";
		
		kprintf("%s penis ist %i/%i mm lang.", str, i, n);
		uart_write(10);
		
		
	}


}
