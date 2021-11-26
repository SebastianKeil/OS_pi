#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>

#define SYS_TIMER 2
#define UART_INT 25

//int print_register_dump; //global

unsigned int sys_timer_pending;
unsigned int uart_pending;


void irq(){
	kprintf("irq_handler: now checking pending registers..\n");
	if(print_register_dump){
		//print register 
	}
	
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		kprintf("uart is pending, push char to buffer\n");
		
		//put uart_port->uart_port->dr & 255 in ringbuffer
	}
	
	if(sys_timer_pending){
		kprintf("!%i\n", print_register_dump);
	}
	//eigentlich RÜCKSPRUNG
	while(1);
}
