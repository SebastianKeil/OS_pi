#include <arch/bsp/yellow_led.h>
#include <config.h>
#include <arch/bsp/uart.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/bsp/sys_timer.h>
#include <arch/cpu/check_interrupts.h>




volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	disable_uart_fifo();
	enable_interrupts_timer_and_uart();
	set_timing(TIMER_INTERVAL);
	
	
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
		receive_buffer = uart_read();	
		
		//TODO create Interrupts 
		check_for_interrupts(receive_buffer);
			
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);

	}
	
	
}
