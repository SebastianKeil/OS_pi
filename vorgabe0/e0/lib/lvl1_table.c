#include <lib/utilities.h>
#include <kernel/kprintf.h>

#define SET_BIT (var,pos) ((var) | (1 << pos))
#define NG_BIT 17//0
#define S_BIT 16//0
#define XN_BIT 4//nur lvl1
#define XN_BIT_LVL2 
#define C_BIT 3 //0
#define B_BIT 2 //0
#define PXN_BIT 2//für lvl2 übernehmen
#define AP_BIT 11 //anfang
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

void initalize_mmu(){
	for(int i = 0; i++, i < 4096){
		lvl1_table[i] = 0;
	}
	return;
}

void set_bits(unsigned int ap_0, unsigned int ap_1, unsigned int ap_2, unsigned int xn_bit){

	return;
}
