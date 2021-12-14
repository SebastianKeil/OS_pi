#include <lib/ringbuffer.h>
#include <arch/cpu/check_interrupts.h>

unsigned char received;

void idle_thread(){
	asm volatile ("wfi"); //power saving mode till irq
	if(input_buffer->count > 0){
		received = buffer_pull(input_buffer);
		check_for_interrupts(received);
	}
}
