#include <config.h>
#include <lib/regcheck.h>
#include <lib/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>

#define LOWER_BOUND_UPPER_CASE 65
#define UPPER_BOUND_UPPER_CASE 90
#define LINEFEED (unsigned char) 10
#define K_ASCII 75

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(unsigned int ticks){
	for(volatile unsigned int i = 0; i < ticks; i++){}
	return;
}

void print_answer(unsigned char *input){
	for(int i = 0; i < 3; i++){
		syscall_put_char(*input);
		syscall_sleep_thread(5);
	}
	return;
}

void print_answer_uppercase(unsigned char *input){
	for(int i = 0; i < 5; i++){
		syscall_put_char(*input);
		sleep(BUSY_WAIT_COUNTER*20);
		
	}
	syscall_put_char(LINEFEED);
	return;
}

void unterprogramm(unsigned char *input){
	unsigned char character = *input;
	unsigned int range = (unsigned int)character;

	if (range >= LOWER_BOUND_UPPER_CASE && range <= UPPER_BOUND_UPPER_CASE){
		if(range == K_ASCII){
			//lesender Zugriff auf Kernel-Stack
			asm volatile 	("ldr r0, =0x3FFFF8\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			syscall_kill_thread();	
			return;
		}
		print_answer_uppercase(input);
		syscall_kill_thread();	
		return;
	}
	
	
	switch(character){
		case 'n':			//lesender Zugriff auf Null-Pointer
			asm volatile 	("ldr r0, =0x0\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'p': 			//Sprung auf Null-Pointer
			asm volatile 	("ldr pc, =0x0");
			break;

		case 'd':			//lesender Zugriff auf Kernel-Daten
			asm volatile 	("ldr r0, =0x300000\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'k':			//lesender Zugriff auf Kernel-Code
			asm volatile 	("ldr r0, =0x100000\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;			

		case 'g':			//lesender Zugriff auf Peripherie-Gerät, z.B. UART
			asm volatile 	("ldr r0, =0x3f201000\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'c':			//schreibender Zugriff auf eigenen Code
			asm volatile 	("ldr r0, =0x500020\t\n"
							"str r0, [r0]\t\n"
							: : : "r0");
			break;

		case 's':			//Stack-Overflow
			asm volatile 	("loop1:\t\n" 
							"push {r0-r12}\t\n"
							"bl loop1\t\n"
							: : : "memory");			
			break;

		case 'u':			//lesender Zugriff auf nicht zugeordnete Adresse
			asm volatile 	("ldr r0, =0x5a00000\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'x':			//Sprung auf eigene Daten oder Stack
			asm volatile 	("ldr pc, =0x700000");
			break;
		default:
			print_answer(input);
			syscall_kill_thread();
			break;
	}
	
}

/*
			asm volatile 	("ldr r0, =0x500020\t\n"
							"str r0, [r0]\t\n"
							: : : "r0");
*/



