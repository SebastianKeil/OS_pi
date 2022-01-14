#include <kernel/kprintf.h>
#include <config.h>
#include <lib/regcheck.h>
#include <lib/unterprogramm.h>

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(unsigned int ticks){
	for(volatile unsigned int i = 0; i < ticks; i++){}
	return;
}

void print_answer(unsigned char *input){
	for(int i = 0; i < 10; i++){
		kprintf("%c", *input);
		sleep(BUSY_WAIT_COUNTER*20);
	}
	return;
}

void end_this_thread(){
	asm volatile("svc #69");
}

void unterprogramm(unsigned char *input){
	unsigned char character = *input;
	switch(character){
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
			end_this_thread();
			break;
		default:
			//kprintf("unterprogramm laeuft mit: %c\n", input);
			print_answer(input);
			end_this_thread();
			break;
	}
}
