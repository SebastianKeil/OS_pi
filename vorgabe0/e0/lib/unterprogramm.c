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
	for(int i = 0; i < 5; i++){
		kprintf("%i%c, ", i, input);
		sleep(BUSY_WAIT_COUNTER*20);
	}
	return;
}

void end_this_thread(){
	asm volatile("svc #69");
}

void unterprogramm(unsigned char *input){
	unsigned char print_data = *input;
	//kprintf("unterprogramm laeuft mit: %c\n", input);
	print_answer(print_data);
	end_this_thread();
}
