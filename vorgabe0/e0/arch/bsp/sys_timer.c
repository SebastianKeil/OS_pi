#include <kernel/kprintf.h>

#define SYS_TIMER_BASE (0x7E003000 - 0x3F000000)

struct sys_timer {
	//0x0
	unsigned int cs;
	unsigned int clo;
	unsigned int chi;
	unsigned int c0;
	unsigned int c1;
	unsigned int c2;
	unsigned int c3;
};

static volatile struct sys_timer * const sys_timer_port = (struct sys_timer *)SYS_TIMER_BASE;

void set_timing(){
	
}


