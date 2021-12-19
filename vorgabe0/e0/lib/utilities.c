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
#define LINK_REGISTER 21
#define DFSR_RW_BIT 11

int define_mode(unsigned int spsr){
	unsigned int mode = (spsr & 31); 
	return mode;
}

//MRC p15, 0, <Rt>, c6, c0, 2 ; Read Instruction Fault Address Register
unsigned int get_ifar(){
	unsigned int value;
	asm volatile ("mrc p15,0,r0,c6,c0,2":"=r"(value):);
	return value;
}	
//MRC p15, 0, <Rt>, c5, c0, 1 ; Read Instruction Fault Status Register
unsigned int get_ifsr(){
	unsigned int value;
	asm volatile ("mrc p15,0,r0,c5,c0,1":"=r"(value):);
	return value;
}

void print_ifsr_status(unsigned int ifsr){
	unsigned int status_03  = 15 & ifsr;
	unsigned int status_4	= 512 & ifsr;
	status_4 = (status_4 >> 5);
	unsigned int status_04 = status_4 + status_03;

	switch (status_04) { // DIESE PRINTS SIND NICHT KORREKT, ZEITMANGEL
	case 0:
		kprintf("No function, reset value");
		break;
	case 1:
		kprintf("No function");
		break;
	case 2:
		kprintf("Debug event Fault");
		break;
	case 3:
		kprintf("Access Flag fault on Section");
		break;
	case 4:
		kprintf("No function");
		break;
	case 5:
		kprintf("Translation fault on Section");
		break;
	case 6:
		kprintf("Access Flag fault on Page");
		break;
	case 7:
		kprintf("Translation fault on Page");
		break;
	case 8:
		kprintf("Precise External Abort");
		break;
	case 9:
		kprintf("Domain fault on Section");
		break;
	case 10:
		kprintf("No function");
		break;
	case 11:
		kprintf("Domain fault on Page");
		break;
	case 12:
		kprintf("External abort on Section");
		break;
	case 13:
		kprintf("Permission fault on Section");
		break;
	case 14:
		kprintf("External abort on Page");
		break;
	case 15:
		kprintf("Permission fault on Page");
		break;
	case 16:
		kprintf("No function");
		break;
	case 17:
		kprintf("No function");
		break;
	case 18:
		kprintf("No function");
		break;
	case 19:
		kprintf("No function");
		break;
	case 20:
		kprintf("No function");
		break;
	case 21:
		kprintf("No function");
		break;
	case 22:
		kprintf("No function");
		break;
	case 23:
		kprintf("No function");
		break;
	case 24:
		kprintf("No function");
		break;
	case 25:
		kprintf("No function");
		break;
	case 26:
		kprintf("No function");
		break;
	case 27:
		kprintf("No function");
		break;
	case 28:
		kprintf("No function");
		break;
	case 29:
		kprintf("No function");
		break;
	case 30:
		kprintf("No function");
		break;
	case 31:
		kprintf("No function");
		break;
	default:
		kprintf("status not listed");
		break;
	}
}

unsigned int get_dfar(){
	unsigned int value;
	asm volatile ("mrc p15,0,r0,c6,c0,0":"=r"(value):);
	return value;
}
unsigned int get_dfsr(){
	unsigned int value;
	asm volatile ("mrc p15,0,r0, c5, c0, 0":"=r"(value):);
	return value;
}
void print_dfsr_status(unsigned int dfsr){
	unsigned int status_03  = 15 & dfsr;
	unsigned int status_4	= 1024 & dfsr;
	status_4 = (status_4 >> 6);
	unsigned int status_04 = status_4 + status_03;

	switch (status_04) {
	case 0:
		kprintf("No function, reset value");
		break;
	case 1:
		kprintf("Alignment Fault");
		break;
	case 2:
		kprintf("Debug event Fault");
		break;
	case 3:
		kprintf("Access Flag fault on Section");
		break;
	case 4:
		kprintf("Cache maintenance operation fault");
		break;
	case 5:
		kprintf("Translation fault on Section");
		break;
	case 6:
		kprintf("Access Flag fault on Page");
		break;
	case 7:
		kprintf("Translation fault on Page");
		break;
	case 8:
		kprintf("Precise External Abort");
		break;
	case 9:
		kprintf("Domain fault on Section");
		break;
	case 10:
		kprintf("No function");
		break;
	case 11:
		kprintf("Domain fault on Page");
		break;
	case 12:
		kprintf("External abort on Section");
		break;
	case 13:
		kprintf("Permsission fault on Section");
		break;
	case 14:
		kprintf("External abort on Page");
		break;
	case 15:
		kprintf("Permission fault on Page");
		break;
	case 16:
		kprintf("No function");
		break;
	case 17:
		kprintf("No function");
		break;
	case 18:
		kprintf("No function");
		break;
	case 19:
		kprintf("No function");
		break;
	case 20:
		kprintf("No function");
		break;
	case 21:
		kprintf("No function");
		break;
	case 22:
		kprintf("Imprecise External Abort");
		break;
	case 23:
		kprintf("No function");
		break;
	case 24:
		kprintf("No function");
		break;
	case 25:
		kprintf("No function");
		break;
	case 26:
		kprintf("No function");
		break;
	case 27:
		kprintf("No function");
		break;
	case 28:
		kprintf("No function");
		break;
	case 29:
		kprintf("No function");
		break;
	case 30:
		kprintf("No function");
		break;
	case 31:
		kprintf("No function");
		break;
	default:
		kprintf("status not listed");
		break;
	}
}

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

// Art der Exception, Adresse der Exception
// Nur bei Data Abort: Art und Adresse des verursachenden Speicherzugriffs (DFSR/DFAR)
// Nur bei Prefetch Abort: Art und Adresse der verursachenden Instruktion (IFSR/IFAR) 

void print_reg_dump(unsigned int regs[], unsigned int modus){	

	char bitmask_cpsr[9];
	char bitmask_spsr[9];
	char bitmask_spsr_svc[9];
	char bitmask_spsr_abt[9];
	char bitmask_spsr_fiq[9];
	char bitmask_spsr_irq[9];
	char bitmask_spsr_und[9];
	char* cpsr_string = psr_to_bitmask(regs[17], bitmask_cpsr, CPSR);
	char* spsr_string = psr_to_bitmask(regs[18], bitmask_spsr, SPSR);
	char* spsr_svc = psr_to_bitmask(regs[12], bitmask_spsr_svc, SPSR);
	char* spsr_abt = psr_to_bitmask(regs[9], bitmask_spsr_abt, SPSR);
	char* spsr_fiq = psr_to_bitmask(regs[6], bitmask_spsr_fiq, SPSR);
	char* spsr_irq = psr_to_bitmask(regs[3], bitmask_spsr_irq, SPSR);
	char* spsr_und = psr_to_bitmask(regs[0], bitmask_spsr_und, SPSR);
	
	switch(modus){
	// 0 reset
	case 1:
		kprintf("Undefined Instruction an Stelle 0x%08x\n", (regs[LINK_REGISTER]));
		break;
	case 2:
		kprintf("Supervisor Call an Stelle 0x%08x\n", (regs[LINK_REGISTER]));
		break;
	case 3:
		kprintf("Prefetch Abort an Stelle 0x%08x\n", (regs[LINK_REGISTER]));
		unsigned int ifar = get_ifar();
		unsigned int ifsr = get_ifsr();
		kprintf("auf Adresse: 0x%08x", ifar);
		kprintf("Fehler: ");
		print_ifsr_status(ifsr);
		kprintf("\n");
		break;
	case 4:
		kprintf("Data Abort an Stelle 0x%08x\nZugriff: ", (regs[LINK_REGISTER]));
		unsigned int dfar = get_dfar();
		unsigned int dfsr = get_dfsr();
		unsigned int read = ((dfsr) & (1 << DFSR_RW_BIT));
		if (read) {
			kprintf("lesend ");
		} else {
		kprintf("schreibend ");
		kprintf("auf Adresse: 0x%08x\n", dfar);
		kprintf("Fehler: ");
		print_dfsr_status(dfsr);
		kprintf("\n");
		}
	break;
	// 5 unused
	case 6:
		kprintf("IRQ an Stelle 0x%08x\n", (regs[LINK_REGISTER]));
		break;
	case 7:
		kprintf("FIQ an Stelle 0x%08x\n", (regs[LINK_REGISTER]));
		break;
	default:
		return;
		}


	kprintf("###########################################################################\n");
	kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n");
	kprintf("R0:\t0x%08x\tR8:\t0x%08x\n"
			"R1:\t0x%08x\tR9:\t0x%08x\n"
			"R2:\t0x%08x\tR10:\t0x%08x\n"
			"R3:\t0x%08x\tR11:\t0x%08x\n"
			"R4:\t0x%08x\tR12:\t0x%08x\n"
			"R5:\t0x%08x\tSP:\t0x%08x\n"
			"R6:\t0x%08x\tLR:\t0x%08x\n"
			"R7:\t0x%08x\tPC:\t0x%08x\n", regs[22], regs[30], regs[23], regs[31], regs[24], regs[32], regs[25], regs[33], regs[26], regs[34], regs[27], regs[19], regs[28], regs[21], regs[29], regs[20]);
	kprintf(">>> Aktuelle Statusregister (SPSR des aktuellen Modus) <<<\n");
	kprintf("CPSR: %c%c%c%c %c %c%c%c\n",cpsr_string[0], cpsr_string[1], cpsr_string[2], cpsr_string[3], cpsr_string[4], cpsr_string[5], cpsr_string[6], cpsr_string[7]);
	kprintf("SPSR: %c%c%c%c %c %c%c%c\n", spsr_string[0], spsr_string[1], spsr_string[2], spsr_string[3],spsr_string[4], spsr_string[5], spsr_string[6], spsr_string[7]);
	kprintf(">>> Aktuelle modusspezifische Register <<<\n");
	kprintf("\t\tLR\t\tSP\t\tSPSR\n");
	kprintf("User/System:\t%08x,\t%08x\n", regs[16], regs[15]);
	kprintf("Supervisor:\t%08x,\t%08x\t%c%c%c%c %c %c%c%c\n", regs[14], regs[13], spsr_svc[0], spsr_svc[1], spsr_svc[2], spsr_svc[3], spsr_svc[4], spsr_svc[5], spsr_svc[6], spsr_svc[7]);
	kprintf("Abort:\t\t%08x,\t%08x\t%c%c%c%c %c %c%c%c\n", regs[11], regs[10], spsr_abt[0], spsr_abt[1], spsr_abt[2], spsr_abt[3], spsr_abt[4], spsr_abt[5], spsr_abt[6], spsr_abt[7]);
	kprintf("FIQ:\t\t%08x,\t%08x\t%c%c%c%c %c %c%c%c\n", regs[8], regs[7], spsr_fiq[0], spsr_fiq[1], spsr_fiq[2], spsr_fiq[3], spsr_fiq[4], spsr_fiq[5], spsr_fiq[6], spsr_fiq[7]);
	kprintf("IRQ:\t\t%08x,\t%08x\t%c%c%c%c %c %c%c%c\n", regs[5], regs[4], spsr_irq[0], spsr_irq[1], spsr_irq[2], spsr_irq[3], spsr_irq[4], spsr_irq[5], spsr_irq[6], spsr_irq[7]);
	kprintf("Undefined:\t%08x,\t%08x\t%c%c%c%c %c %c%c%c\n", regs[2], regs[1], spsr_und[0], spsr_und[1], spsr_und[2], spsr_und[3], spsr_und[4], spsr_und[5], spsr_und[6], spsr_und[7]);
	kprintf("System angehalten.\n");
	return;
}
