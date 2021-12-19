#include <kernel/kprintf.h>
#include <lib/ringbuffer.h>
#include <config.h>
#include <lib/regcheck.h>
#include <lib/unterprogramm.h>

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(int ticks){
	for(volatile int i = 0; i < ticks; i++){}
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
	unsigned char character = input[0];
	switch(character){
		case 's':
			//supervisor call
			kprintf("test: supervisor interrupt by thread\n");
			asm volatile("svc #1");	 
			break;
		case 'p':
			//prefetch abort
			kprintf("test: prefetch interrupt by thread\n");
			asm volatile("bkpt #1");
			break;
		case 'a':
			//data abort
			kprintf("test: data interrupt by thread\n");
			asm volatile("bkpt #0");
			break;
		case 'u':
			//undefined instruction
			kprintf("test: undefined interrupt by thread\n");
			asm volatile("udf");
			break;
		case 'c':
			//register checker ausführen
			kprintf("test: register checker\n");
			register_checker();
			break;
		default:
			//kprintf("unterprogramm laeuft mit: %c\n", input);
			print_answer(input);
			end_this_thread();
	}
}
