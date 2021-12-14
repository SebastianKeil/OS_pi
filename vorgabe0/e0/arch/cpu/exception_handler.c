#include <lib/utilities.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/sys_timer.h>
#include <arch/bsp/thread_admin.h>
#include <arch/cpu/check_interrupts.h>

#define UND 1
#define SVC 2
#define PRE 3
#define DATA_ABORT 4
// unused 5
#define IRQ 6
#define FIQ 7

#define SYS_TIMER 2
#define UART_INT 25

#define USER_MODE	16 //10000
#define FIQ_MODE	17 //10001
#define IRQ_MODE	18 //10010
#define SVC_MODE	19 //10011
#define ABORT_MODE	23 //10111
#define UND_MODE	27 //11011
#define SYS_MODE	31 //11111

void undefined_instruction(unsigned int regs[35]){
	if(define_mode(regs[17])== USER_MODE){
		//kill_thread();
	} else {
	print_reg_dump(regs, UND);
	while(1);
	}
}
void software_interrupt(unsigned int regs[35]){
	if(define_mode(regs[17])== USER_MODE){
		//kill_thread();
	} else {print_reg_dump(regs, SVC);
	while(1);
	}
}
void prefetch_abort(unsigned int regs[35]){
	if(define_mode(regs[17])== USER_MODE){
		//kill_thread();
	} else {
	print_reg_dump(regs, PRE);
	while(1);
	}
}
void data_abort(unsigned int regs[35]){
	if(define_mode(regs[17])== USER_MODE){
		//kill_thread();
	} else {
	print_reg_dump(regs, DATA_ABORT);
	while(1);
	}
}
void fiq(unsigned int regs[35]){
	if(define_mode(regs[17])== USER_MODE){
		//kill_thread();
	} else {
	print_reg_dump(regs, FIQ);
	while(1);
	}
}

unsigned int sys_timer_pending;
unsigned int uart_pending;
unsigned char uart_data;

void irq(unsigned int regs[]){
	//kprintf("\nirq_handler: there is an irq interrupt!\n");
	if(print_register_dump){
		print_reg_dump(regs, IRQ);
	}
	//kprintf("now checking pending registers..\n");
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		//kprintf("uart is pending, push char to buffer\n");
		uart_data = uart_read();
		buffer_push(uart_data, input_buffer);
		check_for_interrupts(buffer_pull(input_buffer));
		
	}else if(sys_timer_pending){
		kprintf("!\n");
		reset_sys_timer();
		scheduler(regs);
	}
	return;
}
