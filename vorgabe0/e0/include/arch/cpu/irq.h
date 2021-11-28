#ifndef IRQ_H
#define IRQ_H

void irq_test(unsigned int regs[16]);
void irq(unsigned int regs[35]);
void print_reg_dump(unsigned int regs[35]);

#endif
