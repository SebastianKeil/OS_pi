#include<stdarg.h>

extern unsigned char syscall_get_char_asm(void); 
extern void syscall_sleep_thread_asm(void); 

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
	void (*programm)(unsigned char*);
} thread_create_context;

unsigned char received_char;

void syscall_put_char(unsigned char c){

	asm volatile 	("mov r0, %0\t\n"
					"svc #42\t\n"
					: : "r" (c) );
	return;
}

unsigned char syscall_get_char(void){

	received_char =  ();
	return received_char;
}


void syscall_kill_thread(){
	asm volatile 	("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*programm)(unsigned char*)){

	thread_create_context.data = data;
	thread_create_context.count = count;
	thread_create_context.programm = programm;

	asm volatile	(//"stmfd sp!, {r0}\t\n"
					"mov r0, %0\t\n"
					"svc #44\t\n"
					//"ldmfd sp!, {r0}\t\n"
					 : : "r" (&thread_create_context));
}

void syscall_sleep_thread(unsigned int length){
	/*
	asm volatile	(//"stmfd sp!, {r0}\t\n"
					"mov r0, %0\t\n"
					"svc #45\t\n"
					//"ldmfd sp!, {r0}\t\n"
					 : : "r" (length));
	*/
	asm volatile ("mov r0, %0" : : "r" (length));
	syscall_sleep_thread_asm();
	
}
