#ifndef LVL1_TABLE_H
#define LVL1_TABLE_H

void initialize_mmu();
unsigned int set_bits(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit, unsigned int pxn_bit, unsigned int sec_bit);
void set_guard_pages(void);

#endif
