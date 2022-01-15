#include <user/user_lib/user_syscalls.h>
#include <config.h>
#include <lib/regcheck.h>
#include <user/unterprogramm.h>
#include <kernel/kprintf.h>



void masterprogramm(){	
	kprintf("entering masterprogramm..\n");
	unsigned char input = syscall_get_char();
	
	/*
	switch(input){
		case 's':
			//supervisor call
			//kprintf("test: supervisor interrupt by thread\n");
			asm volatile("svc #1");	 
			break;
		case 'p':
			//prefetch abort
			//kprintf("test: prefetch interrupt by thread\n");
			asm volatile("bkpt #2");
			break;
		case 'a':
			//data abort
			//kprintf("test: data abort by thread\n");
			//asm volatile("bkpt #0");
			asm volatile("mov r0, #0x1 \n ldr r0, [r0]");
			break;
		case 'u':
			//undefined instruction
			//kprintf("test: undefined interrupt by thread\n");
			asm volatile("udf");
			break;
		case 'c':
			//register checker ausführen
			//kprintf("test: register checker\n");
			register_checker();
			
			//TODO_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
			//end_this_thread();
			
			break;
		default:
			//kprintf("unterprogramm laeuft mit: %c\n", input);
			syscall_create_thread(input, )
			
			//TODO_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
			//end_this_thread();
			
			break;
	}
	*/
}



