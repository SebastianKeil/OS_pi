#include <kernel/kprintf.h>
#include "arch/cpu/shared.h"

int print_register_dump;

void check_for_interrupts(char receive_buffer){
	switch(receive_buffer){
		case 's':
			//supervisor call
			kprintf("supervisor\n");
					 
			break;
		case 'a':
			//data abort
			kprintf("data\n");
			break;
		case 'u':
			//undefined instruction
			kprintf("undefined\n");
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
				kprintf("register checker on\n");
			}else{ 
				print_register_dump = 0;
				kprintf("register checker off\n");
			}
			break;
		case 'e':
			//interaktive unterprogram
			kprintf("unterprogram\n");
			break;
		case 'c':
			//register checker ausführen
			kprintf("register checker\n");
			break;
	}
}
