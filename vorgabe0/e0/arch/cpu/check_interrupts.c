#include <kernel/kprintf.h>

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
	}
}
