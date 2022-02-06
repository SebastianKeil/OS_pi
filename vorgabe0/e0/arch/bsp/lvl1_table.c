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

/* 	Aufbau MMU:

AP[2:0] priv. Modus User-Modus 	Kommentar
000 	No access 	No access 	kein Zugriff
001 	Read/write 	No access 	System-Zugriff
101 	Read-only 	No access 	System-Nur-Lesen
111 	Read-only 	Read-only 	Beide-Nur-Lesen
010 	Read/write 	Read-only 	Nur-Lesen
011 	Read/write 	Read/write 	Vollzugriff

L1:
	Fault:		letzte Bits 00
	Zeiger:		letzte Bits 01 zeigt auf L2-Tabelle
	Section:	letzte Bits 1* 

L2:
	Fault:		letzte Bits 00
	Large:		letzte Bits 01 Bits sitzen evtl anders!!!
	Small:		letzte Bits 1*
 */


/* 	MEMORY LAYOUT
	sp_svc 0x3FFFF8 	4194296
	sp_und 0x3FEFF8 	  +4096 = 4190200
	sp_abt 0x3FDFF8  	  +4096 = 4186104
	sp_irq 0x3FCFF8  	  +4096 = 4182008
	sp_fiq 0x3FBFF8  	  +4096 = 4177912
	
	sp_usr 0x7FFFF8    			= 8388600
*/

//#define USER_STACK_BASE 0x7FEFF8
//#define USER_STACK_SIZE 0x1000
//tcbs[i].sp = USER_STACK_BASE - (i * USER_STACK_SIZE);



static unsigned int lvl1_table[4096] __attribute__((aligned(0x4000)));
static unsigned int lvl2_tables[32][256] __attribute__((aligned(0x4000)));


void set_guard_pages(){
	for(int i = 0; i < 32; i++){
		
	}
}

unsigned int set_bits_lvl1(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit, unsigned int pxn_bit, unsigned int sec_bit){

	temp = (ap_0 == 0) ? temp : SET_BIT_1(temp, AP0_BIT);
	temp = (ap_1 == 0) ? temp : SET_BIT_1(temp, AP1_BIT);
	temp = (ap_2 == 0) ? temp : SET_BIT_1(temp, AP2_BIT);
	temp = (xn_bit == 0) ? temp : SET_BIT_1(temp, XN_BIT);
	temp = (pxn_bit == 0) ? temp : SET_BIT_1(temp, PXN_BIT);
	temp = (sec_bit == 0) ? temp : SET_BIT_1(temp, SEC_BIT);

	return temp;
}

unsigned int set_l2(unsigned int lvl2_table[256]){
	for(int i = 0; i < 256; i++){
		temp = 1024 * 4 * i
		lvl2_table[i] = set_bits_lvl2(temp, 1, 1, 0, 1, 1);
		if(i == 255){lvl2_table[i] = set_bits_lvl2(temp, 0, 0, 0, 0, 1);}
	}
}


unsigned int set_bits_lvl2(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int small_page_bit, unsigned int xn_bit){

	temp = (ap_0 == 0) ? temp : SET_BIT_1(temp, AP0_LVL2_BIT);
	temp = (ap_1 == 0) ? temp : SET_BIT_1(temp, AP1_LVL2_BIT);
	temp = (ap_2 == 0) ? temp : SET_BIT_1(temp, AP2_LVL2_BIT);
	temp = (small_page_bit == 0) ? temp : SET_BIT_1(temp, SMALL_PAGE_BIT);
	temp = (xn_bit == 0) ? temp : SET_BIT_1(temp, XN_LVL2_BIT);

	return temp;
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
			unsigned int lvl2_address = &lvl2_tables[i - 9][0];
			lvl2_address = SET_BIT_1(lvl2_address, 2);
			lvl1_table[i] = SET_BIT_1(lvl2_address, 0);
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

/*

00000000000000000000000000000000
0000000000      A   AA     X  SP
00000000000000000000000000000010
00000000100000001000010000000010
00000000100000001000010000010010
00000000001100000000010000010010
00000000010000001000110000000011
00000000010100001000110000010010
00000000011000000000110000010010
00000000011100000000110000010010
00000000100000000000000000000000
00000000100100000000000000000000


000000000000000000000010, 
000100001000010000000010, 
001000001000010000010010, 
001100000000010000010010, 
010000001000110000000011, 
010100001000110000010010, 
011000000000110000010010, 
011100000000110000010010, 
100000000000000000000000, 
100100000000000000000000, 
 0 <repeats 4086 times>}




*/
