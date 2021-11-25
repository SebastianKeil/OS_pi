#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>



void irq(){
	kprintf("irq_handler: now checking pending registers!");
	//interrupt_ctrl_port->pending1
	while(1);
	//
}
