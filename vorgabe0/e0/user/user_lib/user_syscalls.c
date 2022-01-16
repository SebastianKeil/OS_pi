#include<stdarg.h>
#include <kernel/kprintf.h>

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



void syscall_put_char(unsigned char c){
	asm volatile ("svc #42");
}


/*inline-ass beispiel aus VL3:

int add(int a, int b){
asm ("add %0, %0, %1" : "+r" (a) : "r" (b));
return a;
}*/

unsigned char syscall_get_char(void){
	unsigned char received_char;
	
	//hier: speicher $r0 auf stack??	<-----------------------,
	//															|
	asm volatile ("svc #43");//									|
	//OS gibt uns den char in $r0. dann in variable laden:		|
	//asm("mov %1, %0" : "+r" (received_char):); richtig???		|
	//															|
	//evtl hier: $r0 vom stack wiederherstellen!	<-----------'
	
	return received_char;
}

void syscall_kill_thread(){
	asm volatile("svc #69");
}

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*)){
	thread_create_context.data = data;
	thread_create_context.count = count;
	thread_create_context.unterprogramm = unterprogramm;
	
	//evtl $r0 auf stack wegspeicher!?	<-----------------------------------------------,
	//jetzt thread_create_context* (<-pointer!) in $r0 legen							|
	//in der svc behandlung werden die daten gebraucht um thread_create aufzurufen		|
	//asm("mov %1, %0" : : "r" (&thread_create_context)); richtig???					|
	asm volatile("svc #44");//															|
	//$r0 vom stack wiederherstellen !?	<-----------------------------------------------'
	
}

void syscall_sleep_thread(){
	asm volatile ("svc #45");
}
