#include <lib/utilities.h>
#include <kernel/kprintf.h>

#define SET_BIT_1(var,pos) ((var) |  (1 << pos))
#define SET_BIT_0(var,pos) ((var) & ~(1 << pos))
#define NG_BIT 17//0
#define S_BIT 16//0
#define XN_BIT 4//nur lvl1
#define XN_BIT_LVL2
#define C_BIT 3 //0
#define B_BIT 2 //0
#define PXN_BIT 0//für lvl2 übernehmen
#define AP0_BIT 10
#define AP1_BIT 11
#define AP2_BIT 15
#define TEX_BIT 14 //anfang 0

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

static unsigned int lvl1_table[4096] __attribute__((aligned(0x4000)));

unsigned int set_bits(unsigned int temp, unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit, unsigned int pxn_bit, unsigned int sec_bit){
	temp = (ap_0 == 0) ? temp : SET_BIT_1(temp, AP0_BIT);
	temp = (ap_1 == 0) ? temp : SET_BIT_1(temp, AP1_BIT);
	temp = (ap_2 == 0) ? temp : SET_BIT_1(temp, AP2_BIT);
	temp = (xn_bit == 0) ? temp : SET_BIT_1(temp, XN_BIT);
	temp = (pxn_bit == 0) ? temp : SET_BIT_1(temp, PXN_BIT);
	temp = (sec_bit == 0) ? temp : SET_BIT_1(temp, 1);

	return temp;
}

void initialize_mmu(){

	asm volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r" (&lvl1_table));
	for(int i = 0; i < 2047; i++){
		 unsigned int temp = 1024 * 1024 * i;
		 //temp |= (3 << 10);
		 lvl1_table[i] = set_bits(temp, 1, 1, 0, 0, 0, 1);
	}
	return;
}
