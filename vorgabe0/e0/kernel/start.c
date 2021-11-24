#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <arch/cpu/check_interrupts.h>
#include <kernel/kprintf.h>
#include <config.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

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


	test_kprintf();


	// Endless counter
	for (;;) {
		increment_counter();
		receive_buffer = uart_read();	
		
		//TODO create Interrupts 
		check_for_interrupts(receive_buffer);
			
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);

	}
	
	
}
