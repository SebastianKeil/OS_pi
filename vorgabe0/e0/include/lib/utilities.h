#include <config.h>
#include <kernel/kprintf.h> 

#ifndef _UTILITIES_H
#define _UTILITIES_H

char* psr_to_bitmask(unsigned int reg, char output[], unsigned int spsr_flag);
void print_reg_dump(unsigned int regs[], unsigned int modus);

#endif
