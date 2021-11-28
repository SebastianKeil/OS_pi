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


void print_reg_dump(unsigned int regs[]){				
	kprintf("###########################################################################\n");
		kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n");
		kprintf("R0:\t0x%08x\tR8:\t0x%08x\n"
				"R1:\t0x%08x\tR9:\t0x%08x\n"
				"R2:\t0x%08x\tR10:\t0x%08x\n"
				"R3:\t0x%08x\tR11:\t0x%08x\n"
				"R4:\t0x%08x\tR12:\t0x%08x\n"
				"R5:\t0x%08x\tSP:\t0x%08x\n"
				"R6:\t0x%08x\tLR:\t0x%08x\n"
				"R7:\t0x%08x\tPC:\t0x%08x\n", regs[19], regs[27], regs[20], regs[28], regs[21], regs[29], regs[22], regs[30], regs[23], regs[31], regs[24], regs[32], regs[25], regs[33], regs[26], regs[34]);
		kprintf(">>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
		kprintf("CPSR: %08x\n", regs[18]);
		kprintf("SPSR: %08x\n", regs[17]);
		kprintf(">>> Aktuelle modusspezifische Register <<<\n");
		kprintf("\t\tLR\t\tSP\t\tSPSR\n");
		kprintf("User/System:\t%08x,\t%08x\n", regs[16], regs[15]);
		kprintf("Supervisor:\t%08x,\t%08x\t%08x\n", regs[14], regs[13], regs[12]);
		kprintf("Abort:\t\t%08x,\t%08x\t%08x\n", regs[11], regs[10], regs[9]);
		kprintf("FIQ:\t\t%08x,\t%08x\t%08x\n", regs[8], regs[7], regs[6]);
		kprintf("IRQ:\t\t%08x,\t%08x\t%08x\n", regs[5], regs[4], regs[3]);
		kprintf("Undefined:\t%08x,\t%08x\t%08x\n", regs[2], regs[1], regs[0]);
		kprintf("System angehalten.\n");
}

void irq(unsigned int regs[35]){
	if(print_register_dump){
		print_reg_dump(regs);
	}
	
	kprintf("\nirq_handler: now checking pending registers..\n");
	get_pending_status(&sys_timer_pending, &uart_pending);
	if(uart_pending){
		kprintf("uart is pending, push char to buffer\n");
		uart_data = uart_read();
		buffer_push(uart_data, input_buffer);
		reset_uart_interrupt();
		
	}
	
	if(sys_timer_pending){
		kprintf("!%i\n", print_register_dump);
		reset_sys_timer();
	}
	return;
	//eigentlich RÜCKSPRUNG
	//while(1);
}

void irq_test(unsigned int regs[13]){
	kprintf("###########################################################################\n");
	kprintf(">>> Registerschnappschuss (TEST TEST TEST) <<<\n");
	kprintf("R0:\t0x%08x\tR8:\t0x%08x\n"
			"R1:\t0x%08x\tR9:\t0x%08x\n"
			"R2:\t0x%08x\tR10:\t0x%08x\n"
			"R3:\t0x%08x\tR11:\t0x%08x\n"
			"R4:\t0x%08x\tR12:\t0x%08x\n"
			"R5:\t0x%08x\tSP:\t0x%08x\n"
			"R6:\t0x%08x\tLR:\t0x%08x\n"
			"R7:\t0x%08x\tPC:\t0x%08x\n", regs[0], regs[8], regs[1], regs[9], regs[2], regs[10], regs[3], regs[11], regs[4], regs[12], regs[5], regs[13], regs[6], regs[14], regs[7], regs[15]);
	kprintf("\n");
}
