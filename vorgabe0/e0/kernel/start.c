#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	//char *str1 = "Es wurde folgender Charakter eingegeben: ";
	char *str2 = "ASCII:";

	yellow_on();
	
	uart_write(10);
	uart_write(10);
	kprintf("*****************************************************");
	uart_write(10);
	kprintf("********** zum beenden: erst str+a, dann x **********");
	uart_write(10);
	kprintf("*****************************************************");
	uart_write(10);
	uart_write(10);
	kprintf("Hallo ich bin der Kernel, gib eine Ziffer ein: ");
	uart_write(10);
	uart_write(10);
	

	// Endless counter
	for (;;) {
		increment_counter();
		
		//c, s, x, i, u, p
		
		receive_buffer = uart_read();
		
		kprintf("%s %c, In Hexadezimal: %8x, In Dezimal: %8i", str2, 
					receive_buffer, receive_buffer, receive_buffer);
		uart_write(10);
		kprintf("test for negative: %08x", -269);
		uart_write(10);
		
		
	}


}
