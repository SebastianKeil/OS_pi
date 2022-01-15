#include<stdarg.h>
#include <kernel/kprintf.h>

/*
syscall_put_char()					->	asm volatile("svc #42");		
syscall_get_char()					->	asm volatile("svc #43");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #44");
syscall_sleep_thread()				->	asm volatile("svc #45");
*/

//struct { };



void syscall_put_char(unsigned char c){
	asm volatile ("svc #42");
}


/*int add(int a, int b){
asm ("add %0, %0, %1" : "+r" (a) : "r" (b));
return a;
}
*/
unsigned char syscall_get_char(void){
	unsigned char received_char;
	asm volatile ("svc #43");
	//asm("mov %1, %0")
	
}

void syscall_kill_thread(){
	asm volatile("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(), unsigned int regs[]){
	asm volatile("svc #44");
}

void syscall_sleep_thread(){
	asm volatile ("svc #45");
}
