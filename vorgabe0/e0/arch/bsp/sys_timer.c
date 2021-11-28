#include <kernel/kprintf.h>
#include <config.h>

#define SYS_TIMER_BASE (0x7E003000 - 0x3F000000)

struct sys_timer {
	//0x0
	unsigned int cs;
	unsigned int clo;	//free-running-counter
	unsigned int chi;	//free-running-counter
	unsigned int c0;	
	unsigned int c1;	//compare register
	unsigned int c2;
	unsigned int c3;
};

static volatile struct sys_timer * const sys_timer_port = (struct sys_timer *)SYS_TIMER_BASE;
unsigned int ticks_till_interrupt;

void set_timing(int time){
	ticks_till_interrupt = time;
	sys_timer_port->c1 = time;
	kprintf("SYSTEM TIMER SET TO %i\n", sys_timer_port->c1);
}

void print_timer_setting(){
	kprintf("sys_timer_c1 set to: %i\n", sys_timer_port->c1);
	kprintf("sys_timer cs: %i\n", sys_timer_port->cs);
}

void reset_sys_timer(){
	sys_timer_port->cs = (1 << 1); //reset cs: M1 Bit
	sys_timer_port->c1 = sys_timer_port->c1 + ticks_till_interrupt;	//reset for next tick
	kprintf("SYS_TIMER RESET TO: %i\n", sys_timer_port->c1);
}


