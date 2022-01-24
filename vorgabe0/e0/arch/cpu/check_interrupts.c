#include <kernel/kprintf.h>
#include <lib/regcheck.h>
#include <arch/cpu/shared.h>
#include <user/unterprogramm.h>
#include <kernel/thread_admin.h>

int print_register_dump;
unsigned char receive_buffer;

void check_for_interrupts(unsigned char data, unsigned int regs[35]){
	unsigned char receive_buffer = data;
	switch(receive_buffer){
		case 'S':
			//supervisor call
			//kprintf("test: supervisor interrupt ?from kernel?\n");
			asm volatile("svc #1");
					 
			break;
		case 'A':
			//data abort
			//kprintf("test: data interrupt from kernel\n");
			//asm volatile("bkpt #0");
			asm volatile("mov r0, #0x1 \n ldr r0, [r0]");
			break;
		case 'U':
			//undefined instruction
			//kprintf("test: undefined interrupt from kernel\n");
			asm volatile("udf");
			break;
		case 'P':
			//prefetch abort
			//kprintf("test: prefetch interrupt from kernel\n");
			asm volatile("bkpt #0");
			//asm ("b 0xFFFFFFFF");
			break;
		case 'd':
			//register dump on-off-switch
			//set global variable 
			if(print_register_dump == 0){ 
				print_register_dump = 1;
				kprintf("register dump on\n");
			}else{ 
				print_register_dump = 0;
				kprintf("register dump off\n");
			}
			break;
			
		default:
			//create_thread(&receive_buffer, 1, &unterprogramm, regs);
			//kprintf("leaving check_interrupts..\n");
			break;
	}
}
