#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/sys_timer.h>

#define SYS_TIMER 2
#define UART_INT 25

//int print_register_dump; //global

unsigned int sys_timer_pending;
unsigned int uart_pending;
unsigned char uart_data;


void irq(){
	kprintf("irq_handler: now checking pending registers..\n");
	if(print_register_dump){
		//print register 
	}
	
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		kprintf("uart is pending, pushing char to buffer\n");
		uart_data = uart_read();
		buffer_push(&uart_data, input_buffer);
		//put uart_port->uart_port->dr & 255 in ringbuffer
	}
	
	if(sys_timer_pending){
		kprintf("!%i\n", print_register_dump);
		reset_sys_timer();
	}
	//eigentlich RÜCKSPRUNG
	//while(1);
}
