#include <kernel/kprintf.h>

void fiq(){
	kprintf("fiq_handler: entering endlessloop\n");
	while(1){}
}
