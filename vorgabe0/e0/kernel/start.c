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
	
	kprintf("\n\n");
	
	/* KERNEL SETUP */
	disable_uart_fifo();
	set_uart_receive_interrupt(1);
	//enable_interrupts_timer();
	//enable_interrupts_uart();
	set_timing(5000000);
	
	
	yellow_on();
	kprintf("\n\n*****************************************************\n");
	kprintf("********** zum beenden: erst str+a, dann x **********\n");
	kprintf("********** s for svc ********************************\n");
	kprintf("********** a for data_abort *************************\n");
	kprintf("********** u for undefined **************************\n");
	kprintf("********** p for prefetch_abort *********************\n");
	kprintf("********** d switches print ON/OFF ******************\n");
	kprintf("*****************************************************\n\n");
	kprintf("Hallo ich bin der Kernel, gib eine Ziffer ein: \n\n");
	kprintf("\n");
	
	
	



	// Endless counter
	for (;;) {
		increment_counter();
		receive_buffer = uart_read();	
		
		//print_timer_setting(); 
		check_for_interrupts(receive_buffer);
		
		//kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);

	}
	
	
}
