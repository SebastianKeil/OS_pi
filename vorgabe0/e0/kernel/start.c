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
	//char *str1 = "Es wurde folgender Charakter eingegeben: ";
	//char *str2 = "ASCII:";

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

	test_kprintf();

	// int a = 0;
	// int b = 67;
	// int c = 427412349;
	// int a_neg =  - a;
	// int b_neg =  - b;
	// int c_neg =  - c;
	// kprintf("Negativer Zahlentest: \n");
	// kprintf("Minus*%p* DEC mit Spaces: *%8p* \n",a ,a_neg);
	// kprintf("Minus*%8p* DEC mit Nullen: *%p*\n",b ,b_neg);
	// kprintf("Minus*%i* DEC: *%08i* \n",c ,c_neg);
	// kprintf("Negativer Zahlentest: \n");
	// uart_write(10);
	// uart_write(10);

	// Endless counter
	for (;;) {
		increment_counter();
		receive_buffer = uart_read();		
		kprintf("Es wurde folgender Charakter eingegeben: %c, In Hexadezimal: %x, In Dezimal: %08i\n", receive_buffer, receive_buffer, receive_buffer);

	}
}
