#include <kernel/kprintf.h>

//Argumente unsigned int regs[18], unsigned int status_flags[10], unsigned int data, unsigned int prefetch

#define N_BIT 31
#define Z_BIT 30
#define C_BIT 29
#define V_BIT 28
#define E_BIT 8
#define I_BIT 6
#define F_BIT 5
#define T_BIT 4


char* translate_to_bitmask(unsigned int reg){
	char output[10];
	for(int i = 0; i < 9; i++){
		output[i] = "_";
	}
	
	if(reg &= (1<<N_BIT)){
		output[0] = 'N';
	}
	if(reg &= (1<<Z_BIT)){
		output[1] = 'Z';
	}
	if(reg &= (1<<C_BIT)){
		output[2] = 'Z';
	}
	if(reg &= (1<<V_BIT)){
		output[3] = 'Z';
	}
	if(reg &= (1<<E_BIT)){
		output[4] = 'Z';
	}
	if(reg &= (1<<I_BIT)){
		output[5] = 'Z';
	}
	if(reg &= (1<<F_BIT)){
		output[6] = 'Z';
	}
	if(reg &= (1<<T_BIT)){
		output[7] = 'Z';
	}
	output[8] = '\\';
	output[9] = '0';
	return output;
}

void software_interrupt(unsigned int regs[35]){

	kprintf("###########################################################################\n");
	kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n");
	kprintf("R0:\t0x%08x\tR8:\t0x%08x\n"
			"R1:\t0x%08x\tR9:\t0x%08x\n"
			"R2:\t0x%08x\tR10:\t0x%08x\n"
			"R3:\t0x%08x\tR11:\t0x%08x\n"
			"R4:\t0x%08x\tR12:\t0x%08x\n"
			"R5:\t0x%08x\tSP:\t0x%08x\n"
			"R6:\t0x%08x\tLR:\t0x%08x\n"
			"R7:\t0x%08x\tPC:\t0x%08x\n", regs[19], regs[27], regs[20], regs[28], regs[21], regs[29], regs[22], regs[30], regs[23], regs[31], regs[24], regs[32], regs[25], regs[33], regs[26], regs[34]);
	kprintf(">>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
	kprintf("CPSR: %s\n", translate_to_bitmask(regs[18]));
	kprintf("SPSR: %08x\n", regs[17]);
	kprintf(">>> Aktuelle modusspezifische Register <<<\n");
	kprintf("\t\tLR\t\tSP\t\tSPSR\n");
	kprintf("User/System:\t%08x,\t%08x\n", regs[16], regs[15]);
	kprintf("Supervisor:\t%08x,\t%08x\t%08x\n", regs[14], regs[13], regs[12]);
	kprintf("Abort:\t\t%08x,\t%08x\t%08x\n", regs[11], regs[10], regs[9]);
	kprintf("FIQ:\t\t%08x,\t%08x\t%08x\n", regs[8], regs[7], regs[6]);
	kprintf("IRQ:\t\t%08x,\t%08x\t%08x\n", regs[5], regs[4], regs[3]);
	kprintf("Undefined:\t%08x,\t%08x\t%08x\n", regs[2], regs[1], regs[0]);
	kprintf("System angehalten.");

	//while(1);
}

/*
###########################################################################
Data Abort an Adresse 0x000080e0
Zugriff: lesend auf Adresse 0x00000001
Fehler: Alignment fault
>>> Registerschnappschuss (aktueller Modus) <<<
R0: 0x00000001 R8: 0x00000000
R1: 0x00000001 R9: 0x07b41ee8
R2: 0x00009cd0 R10: 0x07b419a4
R3: 0xfffffff9 R11: 0x07f46a84
R4: 0x07f91450 R12: 0x00000010
R5: 0x00000000 SP: 0x000d0000
R6: 0x00008000 LR: 0x000080e8
R7: 0x00000000 PC: 0x000082a8
>>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<
CPSR: _ZC_ _ IF_ Abort (0x600000D7)
SPSR: N___ _ _F_ Supervisor (0x80000053)
>>> Aktuelle modusspezifische Register <<<
LR SP SPSR
User/System: 0x9f73dff7 0x000b0000
Supervisor: 0x000087f8 0x000ffff0 _ZC_ _ IF_ Undefined (0x600001db)
Abort: 0x00008574 0x000cff10 N___ _ _F_ Supervisor (0x80000053)
FIQ: 0xdfd94ffb 0x000f0000 _ZC_ _ _F_ Supervisor (0x60000153)
IRQ: 0x00008824 0x000e0000 _ZC_ _ IF_ Supervisor (0x600001d3)
Undefined: 0x5602659b 0x000c0000 N___ _ _F_ Supervisor (0x80000053)
System angehalten.
*/