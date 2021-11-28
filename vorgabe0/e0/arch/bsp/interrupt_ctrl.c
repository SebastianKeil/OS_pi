#include <kernel/kprintf.h>

#define INTERRUPT_CTRL_BASE (0x7E00B200 - 0x3F000000)
#define SYS_TIMER 1
#define UART_INT 25

struct interrupt_ctrl {
	//0x0
	//unsigned int unused0[15];
	unsigned int irq_basic_pending;
	unsigned int irq_pending1;
	unsigned int irq_pending2;
	unsigned int fiq_control;
	unsigned int enable_irq_1;
	unsigned int enable_irq_2;
	unsigned int enable_basic_irq;
	unsigned int disable_irq_1;
	unsigned int disable_irq_2;
	unsigned int disable_basic_irq;	

};

static volatile struct interrupt_ctrl * const interrupt_ctrl_port = (struct interrupt_ctrl *)INTERRUPT_CTRL_BASE;


void enable_interrupts_timer(){
	interrupt_ctrl_port->enable_irq_1 = (1 << SYS_TIMER);
	kprintf("INTERRUPTS FROM SYSTEM_TIMER ENABLED\n");
}

void enable_interrupts_uart(){
	interrupt_ctrl_port->enable_irq_2 = (1 << UART_INT);
	kprintf("INTERRUPTS FROM UART ARE ENABLED\n");
}

void get_pending_status(unsigned int *sys_timer_pending, unsigned int *uart_pending){
	*sys_timer_pending = 	interrupt_ctrl_port->irq_pending1 & (1 << SYS_TIMER);
	*uart_pending = 	interrupt_ctrl_port->irq_pending2 & (1 << UART_INT);
	//kprintf("pending registers checked\n");
}


