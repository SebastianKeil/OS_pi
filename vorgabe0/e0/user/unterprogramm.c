#include <config.h>
#include <lib/regcheck.h>
#include <lib/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>

#define LOWER_BOUND_UPPER_CASE 65
#define UPPER_BOUND_UPPER_CASE 90
#define LINEFEED (unsigned char) 10

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(unsigned int ticks){
	for(volatile unsigned int i = 0; i < ticks; i++){}
	return;
}

void print_answer(unsigned char *input){
	for(int i = 0; i < 3; i++){
		syscall_put_char(*input);
		//sleep(BUSY_WAIT_COUNTER*20);
		syscall_sleep_thread(5);
	}
	return;
}

void print_answer_uppercase(unsigned char *input){
	for(int i = 0; i < 5; i++){
		syscall_put_char(*input);
		//sleep(BUSY_WAIT_COUNTER*20);
		syscall_sleep_thread(1);
		
	}
	syscall_put_char(LINEFEED);
	return;
}

void unterprogramm(unsigned char *input){
	unsigned char character = *input;
	unsigned int range = (unsigned int)character;

	if (range >= LOWER_BOUND_UPPER_CASE && range <= UPPER_BOUND_UPPER_CASE){
		//kprintf("\tgoing to pint uppercase");
		print_answer_uppercase(input);
		syscall_kill_thread();	
		return;
	}else{
		print_answer(input);
		syscall_kill_thread();
	}
	
	/*
	switch(character){
		case 's':
			//supervisor call
			asm volatile("svc #1");	 
			break;
		case 'p':
			//prefetch abort
			asm volatile("bkpt #2");
			break;
		case 'a':
			//data abort
			//asm volatile("bkpt #0");
			asm volatile("mov r0, #0x1 \n ldr r0, [r0]");
			break;
		case 'u':
			//undefined instruction
			asm volatile("udf");
			break;
		case 'c':
			//register checker ausführen
			register_checker();
			syscall_kill_thread();
			break;
		default:
			print_answer(input);
			syscall_kill_thread();
			break;
	}
	*/
	
}




