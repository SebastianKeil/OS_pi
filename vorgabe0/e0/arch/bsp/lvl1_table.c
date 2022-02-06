#include <lib/utilities.h>
#include <kernel/kprintf.h>

#define SET_BIT_1(var,pos) ((var) |  (1 << pos))
#define SET_BIT_0(var,pos) ((var) & ~(1 << pos))
#define NG_BIT 17//0
#define S_BIT 16//0
#define XN_BIT 4//nur lvl1
#define XN_LVL2_BIT 0
#define C_BIT 3 //0
#define B_BIT 2 //0
#define SEC_BIT 1
#define PXN_BIT 0//für lvl2 übernehmen
#define AP0_BIT 10
#define AP1_BIT 11
#define AP2_BIT 15
#define AP0_LVL2_BIT 4
#define AP1_LVL2_BIT 5
#define AP2_LVL2_BIT 9
#define SMALL_PAGE_BIT 1



static unsigned int lvl1_table[4096] __attribute__((aligned(0x4000)));
static unsigned int lvl2_tables[32][256] __attribute__((aligned(0x400)));

unsigned int set_bits_lvl1(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit, unsigned int pxn_bit, unsigned int sec_bit){

	temp = (ap_0 == 0) ? temp : SET_BIT_1(temp, AP0_BIT);
	temp = (ap_1 == 0) ? temp : SET_BIT_1(temp, AP1_BIT);
	temp = (ap_2 == 0) ? temp : SET_BIT_1(temp, AP2_BIT);
	temp = (xn_bit == 0) ? temp : SET_BIT_1(temp, XN_BIT);
	temp = (pxn_bit == 0) ? temp : SET_BIT_1(temp, PXN_BIT);
	temp = (sec_bit == 0) ? temp : SET_BIT_1(temp, SEC_BIT);

	return temp;
}

unsigned int set_bits_lvl2(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int small_page_bit, unsigned int xn_bit){

	temp = (ap_0 == 0) ? temp : SET_BIT_1(temp, AP0_LVL2_BIT);
	temp = (ap_1 == 0) ? temp : SET_BIT_1(temp, AP1_LVL2_BIT);
	temp = (ap_2 == 0) ? temp : SET_BIT_1(temp, AP2_LVL2_BIT);
	temp = (small_page_bit == 0) ? temp : SET_BIT_1(temp, SMALL_PAGE_BIT);
	temp = (xn_bit == 0) ? temp : SET_BIT_1(temp, XN_LVL2_BIT);

	return temp;
}

void initialize_l2_entry(unsigned int lvl2_table[256], unsigned int base_address){
	for(int i = 0; i < 256; i++){
		unsigned int temp  = base_address + (1024 * 4 * i);
		lvl2_table[i] = set_bits_lvl2(temp, 1, 1, 0, 1, 1);
		if(i == 0){lvl2_table[i] = set_bits_lvl2(temp, 0, 0, 0, 0, 1);}
	}
	return;
}

void initialize_mmu(){

	asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r" (&lvl1_table));
	for(unsigned int i = 0; i < 2047; i++){
		unsigned int temp = 1024 * 1024 * i;
		
		//FAULT .INIT	//Sys 		Usr 		
		if(i < 1){lvl1_table[i] = set_bits_lvl1(temp, 0, 0, 0, 0, 0, 0);}
		//TEXT 			//Sys L 	Usr 
		else if(i < 2){lvl1_table[i] = set_bits_lvl1(temp, 1, 0, 1, 0, 0, 1);}	
		//RODATA 		//Sys L   	Usr 	XN
		else if(i < 3){lvl1_table[i] = set_bits_lvl1(temp, 1, 0, 1, 1, 0, 1);}
		//DATA			//Sys L/S 	Usr 	XN
		else if(i < 4){lvl1_table[i] = set_bits_lvl1(temp, 1, 0, 0, 1, 0, 1);}
		//BSS			//Sys L/S 	Usr 	XN
		else if(i < 5){lvl1_table[i] = set_bits_lvl1(temp, 1, 0, 0, 1, 0, 1);}
		//USR_TEXT 		//Sys L   	Usr L 	pXN
		else if(i < 6){lvl1_table[i] = set_bits_lvl1(temp, 1, 1, 1, 0, 1, 1);}
		//USR_RODATA	//Sys L   	Usr L 	XN
		else if(i < 7){lvl1_table[i] = set_bits_lvl1(temp, 1, 1, 1, 1, 0, 1);}	
		//USR_DATA/BSS 	//Sys L/S 	USR L/S XN
		else if(i < 9){lvl1_table[i] = set_bits_lvl1(temp, 1, 1, 0, 1, 0, 1);}
		
		//32TCB STACKS	//Sys L/S	USR L/S XN
		else if(i < 41){
			unsigned int lvl2_address = (unsigned int)&lvl2_tables[i - 9][0];
			lvl2_address = SET_BIT_1(lvl2_address, 2);
			lvl1_table[i] = SET_BIT_1(lvl2_address, 0);
			initialize_l2_entry(lvl2_tables[i - 9],temp);
		}
			
		//HARDWARE		//Sys L/S 	Usr 	XN	
		else if(i > 1007 && i < 1012){lvl1_table[i] = set_bits_lvl1(temp, 1, 0, 0, 1, 0, 1);}

		//FAULT			//Sys 		Usr 	
		else{lvl1_table[i] = set_bits_lvl1(temp, 0, 0, 0, 0, 0, 0);}				 	

		//Vollzugriff
		//lvl1_table[i] = set_bits_lvl1(temp, 1, 1, 0, 0, 0, 1); 	 //Sys L/S 	USR L/S 
	}

	return;
}
