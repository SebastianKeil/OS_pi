#include <kernel/kprintf.h>
#include <lib/regcheck.h>
#include "arch/cpu/shared.h"

int print_register_dump;

void check_for_interrupts(char receive_buffer){
	switch(receive_buffer){
		case 's':
			//supervisor call
			kprintf("supervisor\n");
			asm volatile("svc 0");
					 
			break;
		case 'a':
			//data abort
			kprintf("data\n");
			asm volatile("bkpt #0");
			break;
		case 'u':
			//undefined instruction
			kprintf("undefined\n");
			asm volatile("udf");
			break;
		case 'p':
			//prefetch abort
			kprintf("prefetch\n");
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
			kprintf("unterprogram\n");
			register_checker();
			break;
		case 'c':
			//register checker ausführen
			kprintf("register checker\n");
			register_checker();
			break;
	}
}
