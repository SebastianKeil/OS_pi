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

<<<<<<< HEAD
unsigned char syscall_get_char(){
	unsigned char received_char = syscall_asm_getc();
=======

/*inline-ass beispiel aus VL3:

int add(int a, int b){
asm ("add %0, %0, %1" : "+r" (a) : "r" (b));
return a;
}*/

unsigned char syscall_get_char(void){
	asm volatile 	("stmfd sp!, {r0}");
	kprintf("syscall-libary speaking: we are asking for char\n");
	asm volatile	("svc #43");
	register unsigned char received_char asm ("r0");
	asm volatile 	("ldmfd sp!, {r0}");
	kprintf("syscall-libary speaking: we received this char: %c\n", received_char);
>>>>>>> ea04964d310c486c54e2feb16ce2bbe5ec1fade5
	return received_char;
}

void syscall_kill_thread(){
	asm volatile 	("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*)){
	//fill data struct
	thread_create_context.data = data;
	thread_create_context.count = count;
	thread_create_context.unterprogramm = unterprogramm;
	
	//safe $r0 in stack and store data struct pointer in $r0
	asm volatile	("stmfd sp!, {r0}");
	asm volatile	("mov r0, %0" : : "r" (&thread_create_context));
	//call svc for thread_create
	asm volatile	("svc #44");
	//load $r0 back from stack
	asm volatile 	("ldmfd sp!, {r0}");
}

void syscall_sleep_thread(unsigned int length){
	asm volatile	("stmfd sp!, {r0}");
	asm volatile	("mov r0, %0" : : "r" (length));
	asm volatile	("svc #45");
	asm volatile 	("ldmfd sp!, {r0}");
}
