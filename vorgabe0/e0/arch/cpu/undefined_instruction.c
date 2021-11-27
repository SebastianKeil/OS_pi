#include <kernel/kprintf.h>

void undefined_instruction(){
	kprintf("entering undefined_instruction");
	while(1);
}
