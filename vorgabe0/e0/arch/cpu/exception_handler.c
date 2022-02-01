#include <lib/utilities.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/sys_timer.h>
#include <kernel/thread_admin.h>
#include <arch/cpu/check_interrupts.h>


/*	Layout vom regs[35] Array: R0=regs[22]
	regs[34-22]		R12-R0
	regs[21]		LR
	regs[20]		PC
	regs[19]		SP
	regs[18-0]		Daten für den Registerprint		
*/

#define UND 1
#define SVC 2
#define PRE 3
#define DATA_ABORT 4
// unused 5
#define IRQ 6
#define FIQ 7

#define SYS_TIMER 2
#define UART_INT 25
#define KILL_THREAD 69

#define BIT_MASK_24 16777215
#define USER_MODE	16 //10000
#define FIQ_MODE	17 //10001
#define IRQ_MODE	18 //10010
#define SVC_MODE	19 //10011
#define ABORT_MODE	23 //10111
#define UND_MODE	27 //11011
#define SYS_MODE	31 //11111

struct _thread_create_context{ 
	unsigned char* data;
	unsigned int count;
	void (*unterprogramm)(unsigned char*);
};

unsigned int get_imm(unsigned int instruction, unsigned int bit_mask){
	unsigned int svc_imm = instruction & bit_mask;
	return svc_imm;
}

void undefined_instruction(unsigned int regs[35]){
	if(define_mode(regs[17]) == USER_MODE){
		kprintf("kill thread because undefined instruction\n");
		kill_thread(regs);
	} else {
	print_reg_dump(regs, UND);
	while(1);
	}
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
syscall_put_char(unsigned char c)	->	asm volatile("svc #42");		
syscall_get_char()					->	asm volatile("svc #43");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #44");
syscall_sleep_thread()				->	asm volatile("svc #45");
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_*/

void software_interrupt(unsigned int regs[35]){
	if(define_mode(regs[17]) == USER_MODE){
	
		unsigned int sleep_time;
		unsigned char received_char;
		unsigned char char_send;
		struct _thread_create_context *_thread_create_context_ptr;
		
		
		unsigned int svc_imm = get_imm(*(unsigned int*)(regs[21] - 4), BIT_MASK_24);
		
		switch(svc_imm){
			case 42:
				kprintf("software_interrupt 42: \n\tcurrent thread offering char %c\n", (unsigned char) regs[22]);
				//in $r0 liegt char für kprintf()
				char_send = (unsigned char) regs[22];
				kprintf("%c", char_send);
				break;
				
			case 43:
				kprintf("software_interrupt 43: \n\tcurrent thread asking for char\n");
				kprintf("\tstack of curr: %p\n", regs[15]);
				//char must be in $r0 when returning
				if(uart_input_buffer.count > 0){
					received_char = buffer_pull(&uart_input_buffer);
					regs[22] = (unsigned int) received_char;
					return;
				} else {
					kprintf("\tbuffer count: %i, thread has to wait\n\n", uart_input_buffer.count);
					wait_thread(0, regs);
				}
				
				break;
				
			case 69:
				kprintf("kill me!\n");
				kill_thread(regs);
				break;
				
			case 44:
				_thread_create_context_ptr = (struct _thread_create_context*) regs[22];
				
				kprintf("software_interrupt44: \n\tcreate thread with %c\n", *(_thread_create_context_ptr->data));
				
				create_thread(_thread_create_context_ptr->data, _thread_create_context_ptr->count, _thread_create_context_ptr->unterprogramm, regs);
				break;
				
			case 45:
				kprintf("make me sleep!\n");
				sleep_time = regs[22];
				if (sleep_time == 0){
					sleep_time = 1;
				}
				wait_thread(sleep_time, regs);
				break;
		}
	} else {
		print_reg_dump(regs, SVC);
		while(1);
	}
	return;
}


void prefetch_abort(unsigned int regs[35]){
	if(define_mode(regs[17]) == USER_MODE){
		kprintf("kill thread because prefetch abort\n");
		print_reg_dump(regs, PRE); //remove!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		kill_thread(regs);
	} else {
	print_reg_dump(regs, PRE);
	while(1);
	}
}
void data_abort(unsigned int regs[35]){
	if(define_mode(regs[17]) == USER_MODE){
		kprintf("kill thread because data abort\n");
		kill_thread(regs);
	} else {
	print_reg_dump(regs, DATA_ABORT);
	while(1);
	}
}
void fiq(unsigned int regs[35]){
	if(define_mode(regs[17]) == USER_MODE){
		kprintf("kill thread because fiq\n");
		kill_thread(regs);
	} else {
	print_reg_dump(regs, FIQ);
	while(1);
	}
}

unsigned int sys_timer_pending;
unsigned int uart_pending;
unsigned char uart_data;

void irq(unsigned int regs[]){
	if(print_register_dump){
		print_reg_dump(regs, IRQ);
	}
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		uart_data = uart_read();
		kprintf("irq: \n\t[%c] was pressed, check if any threads are waiting\n", uart_data);
		buffer_push(uart_data, &uart_input_buffer);
		check_for_waiting_threads(regs);
		kprintf("irq again: \n\tin registers[0] (regs[22]) is %c\n", regs[22]);
		
	}else if(sys_timer_pending){
		kprintf("!");
		reset_sys_timer();
		scheduler(regs);
	}
	return;
}
