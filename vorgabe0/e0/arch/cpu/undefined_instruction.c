#include <kernel/kprintf.h>

void undefined_instruction(){
	kprintf("pimmel");
	while(1);
}
