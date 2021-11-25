#include <kernel/kprintf.h>

void data_abort(unsigned int regs[14]){
//	kprintf("Data Abort an Adresse %08x", );
//	kprintf("Zugriff: lesend auf Adresse %08x", data);
	kprintf("Fehler: Alignment fault");
	}
