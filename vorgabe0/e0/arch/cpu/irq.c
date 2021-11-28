#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/sys_timer.h>
#include <lib/utilities.h>

#define SYS_TIMER 2
#define UART_INT 25

//int print_register_dump; //global

unsigned int sys_timer_pending;
unsigned int uart_pending;
unsigned char uart_data;

void irq(unsigned int regs[35]){
	kprintf("\nirq_handler: there is an irq interrupt!\n");
	regs[19] = regs[19] - 8;
	if(print_register_dump){
		print_reg_dump(regs);
	}
	kprintf("now checking pending registers..\n");
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		kprintf("uart is pending, push char to buffer\n");
		uart_data = uart_read();
		buffer_push(uart_data, input_buffer);
		//reset_uart_interrupt();
		
	}else if(sys_timer_pending){
		kprintf("!%i\n", print_register_dump);
		reset_sys_timer();
	}
	return;
	//eigentlich RÜCKSPRUNG
	//while(1);
}