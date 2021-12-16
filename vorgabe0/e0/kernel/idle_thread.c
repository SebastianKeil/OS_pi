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
	//asm volatile ("wfi");
	while(uart_input_buffer.count < 1){
		sleeps(100000000);
	}
}
