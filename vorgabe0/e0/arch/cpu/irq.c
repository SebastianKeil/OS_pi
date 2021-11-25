#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>

#define SYS_TIMER 2
#define UART_INT 25

unsigned int sys_timer_pending;
unsigned int uart_pending;


void irq(){
	kprintf("irq_handler: now checking pending registers..\n");
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		kprintf("uart is pending\n");
		//put uart_port->uart_port->dr & 255 in ringbuffer
	}
	
	if(sys_timer_pending){
		kprintf("!\n");
	}
	//eigentlich RÜCKSPRUNG
	while(1);
}
