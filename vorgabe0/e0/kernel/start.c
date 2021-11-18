#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <kernel/kprintf.h>
#include <config.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	//TODO "kernel richtet sich ein" 
	//	- Speicherlayout überlegen
	
	// entry.S:
	//	- IVT an bestimmte Stelle (Linker)
	//	- auf CoProzessor VBAR einstellen (CP15)
	// 	- dann Assembler Trampolin mit Store Memory und Load Memory
	
	// eigene .c Datei:
	//	- dann C-Handler
	
	//	- dann System timer einstellen für intervall interrupts
	//	- dann Uart read in einen Buffer und 
	//		alle lesenden zugriffe auf uart in diesen buffer umleiten
	//	- dann interrupts über tastendruck einrichten

	yellow_on();
	
	uart_write(10);
	uart_write(10);
	kprintf("*****************************************************");
	uart_write(10);
	kprintf("********** zum beenden: erst str+a, dann x **********");
	uart_write(10);
	kprintf("*****************************************************");
	uart_write(10);
	uart_write(10);
	kprintf("Hallo ich bin der Kernel, gib eine Ziffer ein: ");
	uart_write(10);
	uart_write(10);


	test_kprintf();


	// Endless counter
	for (;;) {
		increment_counter();
		receive_buffer = uart_read();		
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);

	}
	
	
}
