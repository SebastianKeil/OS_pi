#include <config.h>
#include <kernel/kprintf.h> 

#ifndef _UTILITIES_H
#define _UTILITIES_H

int define_mode(unsigned int spsr);
unsigned int get_ifar();
unsigned int get_ifsr();
void print_ifsr_status(unsigned int ifsr);
unsigned int get_dfar();
unsigned int get_dfsr();
void print_dfsr_status(unsigned int dfsr);
char* psr_to_bitmask(unsigned int reg, char output[], unsigned int spsr_flag);
void print_reg_dump(unsigned int regs[], unsigned int modus);

#endif
