#ifndef LVL1_TABLE_H
#define LVL1_TABLE_H

unsigned int set_bits_lvl1(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit, unsigned int pxn_bit, unsigned int sec_bit);
unsigned int set_bits_lvl2(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int small_page_bit, unsigned int xn_bit);
void initialize_l2_entry(unsigned int lvl2_table[256], unsigned int temp);
void initialize_mmu();

#endif
