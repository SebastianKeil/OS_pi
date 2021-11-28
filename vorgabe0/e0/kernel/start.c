#include <arch/bsp/yellow_led.h>
#include <config.h>
#include <arch/bsp/uart.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/bsp/sys_timer.h>
#include <arch/cpu/check_interrupts.h>
#include <lib/ringbuffer.h>
#include <arch/cpu/shared.h>




volatile unsigned int counter = 0;
volatile unsigned char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){
	
	kprintf("\n\n");
	kprintf("*****************************************************\n");
	kprintf("******************** KERNEL SETUP *******************\n");
	kprintf("*****************************************************\n\n");
	
	/* KERNEL SETUP */
	print_register_dump = 0;
	
	disable_uart_fifo();
	set_uart_receive_interrupt();
	initialize_buffer(input_buffer);
	//enable_interrupts_uart();

	enable_interrupts_timer();
	set_timing(10000);
	
	
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
		if(input_buffer->count > 0){
			receive_buffer = buffer_pull(input_buffer);
			kprintf("pulled char: %c", receive_buffer);
			//check_for_interrupts(receive_buffer);
		}
		
		
		// HA 1:
		//kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);
		
		/* generating test interrupts:
		receive_buffer = uart_read();
		check_for_interrupts(receive_buffer);
		*/

	}
	
	
}
