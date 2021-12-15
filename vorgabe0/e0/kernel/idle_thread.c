#include <lib/ringbuffer.h>
#include <arch/cpu/check_interrupts.h>
#include <kernel/kprintf.h>
#include <config.h>

unsigned char received;

void sleeps(int ticks){
	for(volatile int i = 0; i < ticks; i++){}
	return;
}

void idle_thread(){
	kprintf("idle_thread: entering power saving mode..\n");
	
	//asm volatile ("wfi"); //power saving mode till irq
	while(input_buffer->count < 1){
		kprintf("power saving!!\n");
		sleeps(100000000);
	}
}
