#include <kernel/kprintf.h>

void data_abort(){
//	kprintf("Data Abort an Adresse %08x", );
//	kprintf("Zugriff: lesend auf Adresse %08x", data);
	kprintf("Fehler: Alignment fault");
	}
