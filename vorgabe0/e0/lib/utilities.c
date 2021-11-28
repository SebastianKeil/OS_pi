#include <kernel/kprintf.h>
#include <config.h>

#define N_BIT 31
#define Z_BIT 30
#define C_BIT 29
#define V_BIT 28
#define E_BIT 9
#define I_BIT 7
#define F_BIT 6
#define T_BIT 5
#define CHECK_BIT(var,pos) ((var) & (1 << pos))
#define SPSR 1
#define CPSR 0

char* psr_to_bitmask(unsigned int reg, char output[], unsigned int spsr_flag){
	for(int i = 0; i < 8; i++){
		output[i] = '_';
	}
	if(CHECK_BIT(reg, N_BIT)){
		output[0] = 'N';
	}
	if(CHECK_BIT(reg,Z_BIT)){
		output[1] = 'Z';
	}
	if(CHECK_BIT(reg,C_BIT)){
		output[2] = 'C';
	}
	if(CHECK_BIT(reg,V_BIT)){
		output[3] = 'V';
	}
	if(CHECK_BIT(reg,E_BIT)){
	    if(spsr_flag){
	 	output[4] = 'E';
	    }
	}
	if(CHECK_BIT(reg,I_BIT)){
		output[5] = 'I';
	}
	if(CHECK_BIT(reg,F_BIT)){
		output[6] = 'F';
	}
	if(CHECK_BIT(reg,T_BIT)){
		output[7] = 'T';
	}
    output[8] = '\0';
    
	return output;
}

void print_reg_dump(unsigned int regs[]){	

	char bitmask_spsr[9];
	//char bitmask_cpsr[8];
	regs[18] = 0x600001d2;
	char* cpsr_string = psr_to_bitmask(regs[18], bitmask_spsr, SPSR);
		
	kprintf("Test für den SPSR: %x \n", regs[18]);
	kprintf("SPSR: %c%c%c%c %c %c%c%c\n",cpsr_string[0], cpsr_string[1], cpsr_string[2], cpsr_string[3],cpsr_string[4], cpsr_string[5], cpsr_string[6], cpsr_string[7]);



  /*
	kprintf("###########!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!####################\n");
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
	kprintf(" Test für den CPSR: %x \n", regs[18]);
	kprintf("CPSR: %c%c%c%c %c %c%c%c\n",cpsr_string[0], cpsr_string[1], cpsr_string[2], cpsr_string[3],cpsr_string[4], cpsr_string[5], cpsr_string[6], cpsr_string[7]);
	kprintf("SPSR: %08x\n", regs[17]);
	kprintf(">>> Aktuelle modusspezifische Register <<<\n");
	kprintf("\t\tLR\t\tSP\t\tSPSR\n");
	kprintf("User/System:\t%08x,\t%08x\n", regs[16], regs[15]);
	kprintf("Supervisor:\t%08x,\t%08x\t%08x\n", regs[14], regs[13], regs[12]);
	kprintf("Abort:\t\t%08x,\t%08x\t%08x\n", regs[11], regs[10], regs[9]);
	kprintf("FIQ:\t\t%08x,\t%08x\t%08x\n", regs[8], regs[7], regs[6]);
	kprintf("IRQ:\t\t%08x,\t%08x\t%08x\n", regs[5], regs[4], regs[3]);
	kprintf("Undefined:\t%08x,\t%08x\t%08x\n", regs[2], regs[1], regs[0]);
	kprintf("System angehalten.\n");
	*/
}