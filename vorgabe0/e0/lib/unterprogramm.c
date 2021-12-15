#include <kernel/kprintf.h>
#include <lib/ringbuffer.h>
#include <config.h>

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(int ticks){
	for(volatile int i = 0; i < ticks; i++){}
	return;
}

void print_answer(unsigned char input){
	for(int i = 0; i < 10; i++){
		kprintf("%c", input);
		sleep(BUSY_WAIT_COUNTER);
	}
	return;
}

void end_this_thread(){
	asm volatile("svc 0");
}

void unterprogramm(unsigned char *input){
	kprintf("unterprogramm laeuft...\n");
	print_answer(*input);
	end_this_thread();
}
