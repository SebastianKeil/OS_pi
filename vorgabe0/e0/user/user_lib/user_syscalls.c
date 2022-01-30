#include<stdarg.h>
#include<kernel/kprintf.h>
#include<syscall_asm_getc.S> 

/*
syscall_put_char()					->	asm volatile("svc #42");		
syscall_get_char()					->	asm volatile("svc #43");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #44");
syscall_sleep_thread()				->	asm volatile("svc #45");
*/

struct thread_create_context{ 
	unsigned char* data;
	unsigned int count;
	void (*unterprogramm)(unsigned char*);
} thread_create_context;

unsigned char received_char;

void syscall_put_char(unsigned char c){
	kprintf("svc_put_char: \n\toffering char\n");
	
	/*
	asm volatile 	("mov r0, %0\t\n"
					"svc #42\t\n"
					: : "r" (c) );
	*/
					
	asm volatile ("mov r0, %0" : : "r" (c) );
	asm volatile ("svc #42");
	return;
}

extern unsigned char syscall_asm_getc();

unsigned char syscall_get_char(){
	unsigned char received_char = syscall_asm_getc();
	return received_char;

void syscall_kill_thread(){
	asm volatile 	("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*)){
	kprintf("svc_create_thread: \n\twurde mit %c aufgerufen\n", *data);
	//fill data struct
	thread_create_context.data = data;
	thread_create_context.count = count;
	thread_create_context.unterprogramm = unterprogramm;
	
	//safe $r0 in stack and store data struct pointer in $r0

	asm volatile	(//"stmfd sp!, {r0}\t\n"
					"mov r0, %0\t\n"
					"svc #44\t\n"
					//"ldmfd sp!, {r0}\t\n"
					 : : "r" (&thread_create_context));
}

void syscall_sleep_thread(unsigned int length){
	asm volatile	(//"stmfd sp!, {r0}\t\n"
					"mov r0, %0\t\n"
					"svc #45\t\n"
					//"ldmfd sp!, {r0}\t\n"
					 : : "r" (length));
}
