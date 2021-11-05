#include <arch/bsp/yellow_led.h>
#include <arch/bsp/uart.h>
#include <lib/kprintf.h>

volatile unsigned int counter = 0;
volatile char receive_buffer;

void increment_counter() {
	counter++;
}


void start_kernel(){

	//char *str1 = "Es wurde folgender Charakter eingegeben: ";
	char *str2 = "ASCII:";

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
	

	/*Functions to test for

	%c – int Argument wird nach unsigned char umgewandelt und als einzelnes Zeichen ausgegeben
	%s – durch const char * Argument referenzierte null-terminierte Zeichenkette wird ausgegeben
	%x – unsigned int Argument wird in hexadezimaler Darstellung ausgegeben
	%i – int Argument wird in dezimaler Darstellung ausgegeben
	%u – unsigned int Argument wird in dezimaler Darstellung ausgegeben
	%p – void * Argument wird in hexadezimaler Darstellung mit dem Prefix 0x ausgegeben
	%% – Ein einfaches %-Zeichen wird ausgegeben

	*/
	const int i = 2147483649;
	int i_neg = - i;
	kprintf("Negativer Zahlentest: \n");
	//kprintf("Minus%i DEC: \nMinus%i DEC mit Spaces: \nMinus%i DEC: \n",i ,i ,i);
	
	kprintf("Minus%i DEC: %i \nMinus%i DEC mit Spaces: %8i \nMinus%i DEC mit Nullen: %08i \n",i ,i_neg ,i , i_neg ,i ,i_neg);
	
	//kprintf("Minus%i DEC: \nMinus%i DEC mit Spaces: \nMinus%i DEC: \n",i ,i ,i);
	//kprintf("Minus%i DEC: \nMinus%i DEC mit Spaces: \nMinus%i DEC: ",i ,i ,i);
	uart_write(10);
	uart_write(10);

	// Endless counter
	for (;;) {
		increment_counter();
		
		//c, s, x, i, u, p
		
		receive_buffer = uart_read();
		
		//kprintf("%s %c, In Hexadezimal: %08x In Dezimal: %8i", 
			//str2, receive_buffer, receive_buffer, receive_buffer);
		kprintf("%s %c\nPointer ohne alles: %p \nPointer mit Leerzeichen: %8p \nHexadec mit Leerzeichen: %8x\nPointer mit Nullen: %08p", 
			str2, receive_buffer, receive_buffer, receive_buffer, receive_buffer);
		uart_write(10);
		//kprintf("test for negative: %8x", 269);
		uart_write(10);
			
	}
}
