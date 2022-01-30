#include<stdarg.h>
#include<kernel/kprintf.h>
#include<user/user_lib/syscall_asm_getc.h>

/*
syscall_put_char()					->	asm volatile("svc #42");		
syscall_get_char()					->	asm volatile("svc #43");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #44");
syscall_sleep_thread()				->	asm volatile("svc #45");
*/

extern unsigned char syscall_asm_getc();

struct thread_create_context{ 
	unsigned char* data; 
	unsigned int count;
	void (*unterprogramm)(unsigned char*);
} thread_create_context;

void syscall_put_char(unsigned char c){
	unsigned int character = (unsigned int)c;
	asm volatile 	("stmfd sp!, {r0}");
	asm volatile	("mov r0, %0" : : "r" (character));
	asm volatile	("svc #42");
	asm volatile 	("ldmfd sp!, {r0}");
}

unsigned char syscall_get_char(){
	unsigned char received_char = syscall_asm_getc();
	return received_char;
}

void syscall_kill_thread(){
	asm volatile 	("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*)){
	
	thread_create_context.data = data;
	thread_create_context.count = count;
	thread_create_context.unterprogramm = unterprogramm;
	kprintf("test1\n");
	asm volatile	("stmfd sp!, {r0}");
	asm volatile	("mov r0, %0" : : "r" (&thread_create_context));
	asm volatile	("svc #44");
	asm volatile 	("ldmfd sp!, {r0}");
}

void syscall_sleep_thread(unsigned int length){
	asm volatile	("stmfd sp!, {r0}");
	asm volatile	("mov r0, %0" : : "r" (length));
	asm volatile	("svc #45");
	asm volatile 	("ldmfd sp!, {r0}");
}
