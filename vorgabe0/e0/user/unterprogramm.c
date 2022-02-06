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
	n: lesender Zugriff auf Null-Pointer.
	p: Sprung auf Null-Pointer.
	d: lesender Zugriff auf Kernel-Daten.
	k: lesender Zugriff auf Kernel-Code.
	K: lesender Zugriff auf Kernel-Stack.
	g: lesender Zugriff auf Peripherie-Gerät, z.B. UART.
	c: schreibender Zugriff auf eigenen Code.
	s: Stack-Overflow.
	u: lesender Zugriff auf nicht zugeordnete Adresse.
	x: Sprung auf eigene Daten oder Stack.
	*/
	
	switch(character){
		case 'n':
			//lesender Zugriff auf Null-Pointer
			kprintf("going to read from NULL");
			unsigned int dummy = *(unsigned int*) 0x0;
			break;
		case 'p':
			//Sprung auf Null-Pointer
			kprintf("going to jump to NULL");
			asm ("ldr pc, =0x0");
			break;
		case 'd':
			//lesender Zugriff auf Kernel-Daten
			break;
		case 'k':
			//lesender Zugriff auf Kernel-Code
			break;
		case 'K':
			//lesender Zugriff auf Kernel-Stack
			break;
		case 'g':
			//lesender Zugriff auf Peripherie-Gerät, z.B. UART
			break;
		case 'c':
			//schreibender Zugriff auf eigenen Code
			break;
		case 's':
			//Stack-Overflow
			break;
		case 'u':
			//lesender Zugriff auf nicht zugeordnete Adresse
			break;
		case 'x':
			//Sprung auf eigene Daten oder Stack
			break;
		default:
			print_answer(input);
			syscall_kill_thread();
			break;
	}
	
}




