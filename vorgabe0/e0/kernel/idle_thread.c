#include <lib/ringbuffer.h>
#include <arch/cpu/check_interrupts.h>
#include <kernel/kprintf.h>

unsigned char received;

void idle_thread(){
	kprintf("idle_thread: entering power saving mode..\n");
	//asm volatile ("wfi"); //power saving mode till irq
	
	while(input_buffer->count = 0){
		kprintf("?\n");
	}
	kprintf("idle_thread: interrupt accured, now checking source..\n");
	if(input_buffer->count > 0){
		kprintf("idle_thread: uart just received a char! lets see what to do..\n");
		received = buffer_pull(input_buffer);
		check_for_interrupts(received);
	}
}
