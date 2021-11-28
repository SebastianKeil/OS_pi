#include <kernel/kprintf.h>
#include <lib/ringbuffer.h>

unsigned char char_for_unterprogramm;
unsigned int buffer_count;

void sleep(unsigned int ticks){
	for(volatile int i = 0; i < ticks; i++){}
	return;
}

void print_answer(unsigned char print_char){
	for(int i = 0; i < 64; i++){
		kprintf("%c", print_char);
		sleep(BUSY_WAIT_COUNTER);
	}
	return;
}

void unterprogramm(){
	kprintf("unterprogramm laeuft...\n");
	for(;;){
		buffer_count = get_buffer_count(input_buffer);
		if(input_buffer->count > 0){
			//kprintf("unterprogramm has received char");
				char_for_unterprogramm = buffer_pull(input_buffer);
				print_answer(char_for_unterprogramm);
		}
	}
}
