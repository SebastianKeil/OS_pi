#include <arch/bsp/yellow_led.h>
#include <config.h>
#include <arch/bsp/uart.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/bsp/sys_timer.h>
#include <arch/cpu/check_interrupts.h>
#include <lib/ringbuffer.h>
#include <arch/cpu/shared.h>
#include <kernel/idle_thread.h>
#include <kernel/thread_admin.h>
#include <user/masterprogramm.h>
#include <user/user_lib/user_syscalls.h>



volatile unsigned int counter = 0;
unsigned char void_char;

void increment_counter() {
	counter++;
}


void start_kernel(){
	kprintf("Pisse \n");
	//SCTLR mmu einschalten
	asm volatile 	("mrc p15, 0, r7, c1, c0, 0\t\n"
					"add r7, #1\t\n"
					"mcr p15, 0, r7, c1, c0, 0" : : : "r7");

	kprintf("\n\n");
	kprintf("*****************************************************\n");
	kprintf("******************** KERNEL SETUP *******************\n");
	kprintf("*****************************************************\n\n");
	
	/* KERNEL SETUP */
	//print_register_dump = 0;
	
	disable_uart_fifo();
	set_uart_receive_interrupt();
	initialize_buffer(&uart_input_buffer);
	enable_interrupts_uart();

	enable_interrupts_timer();
	set_timing(500000); //TIMER_INTERVAL 1000000 // 500000 schnell // 50000 schnellste
	
	init_thread_admin();
	
	yellow_on();
	
	kprintf("\n\n*****************************************************\n");
	kprintf("********** zum beenden: erst str+a, dann x **********\n");
	kprintf("********** Grossbuchstaben für Kernel Interrupts ****\n");
	kprintf("********** Kleinbuchstaben für Thread Interrupts ****\n");
	kprintf("********** S/s for svc ******************************\n");
	kprintf("********** A/a for data_abort ***********************\n");
	kprintf("********** U/u for undefined ************************\n");
	kprintf("********** P/p for prefetch_abort *******************\n");
	kprintf("***** d switches register prints ON/OFF *************\n");
	kprintf("*****************************************************\n\n");
	kprintf("Hallo ich bin der Kernel, starte print-threads: \n\n");
	kprintf("\n");
	
	/*change to user mode*/
		asm ("cps #0x10");
		
		void_char = '#';
		//initial thread
		syscall_create_thread(&void_char, 0, &masterprogramm);
		
		//create_thread_simple(&masterprogramm);


	// Endless counter
	for (;;) {
		increment_counter();
		
		/*change to user mode*/
		//asm ("cps #0x10");
		
		//initial thread
		//create_thread_simple(&masterprogramm);
	}
}




/* HA 1:
kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);
*/

/* HA 2:
if(input_buffer->count > 0){
	received_char = buffer_pull(input_buffer);
	check_for_interrupts(received_char);
}
*/
