#include <kernel/kprintf.h>

//Argumente unsigned int regs[18], unsigned int status_flags[10], unsigned int data, unsigned int prefetch
void software_interrupt(unsigned int regs[18]){
	
	kprintf("###########################################################################\n");
	
		
	kprintf(">>> Registerschnappschuss (aktueller Modus) <<<");
	kprintf("R0:\t%08xR8:\t%08x\n"
			"R1:\t%08x\tR9:\t%08x\n"
			"R2:\t%08x\tR10:\t%08x\n"
			"R3:\t%08x\tR11:\t%08x\n"
			"R4:\t%08x\tR12:\t%08x\n"
			"R5:\t%08x\tSP:\t%08x\n"
			"R6:\t%08x\tLR:\t%08x\n"
			"R7:\t%08x\tPC:\t%08x\n", regs[2], regs[10], regs[3], regs[11], regs[4], regs[12], regs[5], regs[13], regs[6], regs[14], regs[7], regs[15], regs[8], regs[16], regs[9], regs[17]);
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