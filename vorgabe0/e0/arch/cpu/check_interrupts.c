#include <kernel/kprintf.h>
#include <lib/regcheck.h>
#include <arch/cpu/shared.h>

int print_register_dump;

void check_for_interrupts(char receive_buffer){
	switch(receive_buffer){
		case 's':
			//supervisor call
			kprintf("test: supervisor\n");
			asm volatile("svc 0");
					 
			break;
		case 'a':
			//data abort
			kprintf("test: data\n");
			asm volatile("bkpt #0");
			break;
		case 'u':
			//undefined instruction
			kprintf("test: undefined\n");
			asm volatile("udf");
			break;
		case 'p':
			//prefetch abort
			kprintf("test: prefetch\n");
			asm volatile("bkpt #1");
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
		case 'e':
			//interaktive unterprogram
			kprintf("test: unterprogram\n");
			break;
		case 'c':
			//register checker ausführen
			kprintf("test: register checker\n");
			register_checker();
			break;
	}
}
