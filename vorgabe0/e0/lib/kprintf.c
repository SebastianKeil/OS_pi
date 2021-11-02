#include<stdarg.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>

/*void print_integer(int i, int base){
}
*/
void replace_and_write(char *fmt, va_list *ap){
	int i;
	const char *string;
	unsigned int u_int;
	void* ptr;

	switch(*fmt){
	case 'c': //int -> unsigned char
		i = va_arg(*ap, int);
		uart_write(i);
		break;
	case 's': //all the string
		string = va_arg(*ap, const char *);
		while(*string){
			uart_write(*string);
			string ++;
		}
		break;
	case 'x': //unsigned int -> in hex
		u_int = va_arg(*ap, unsigned int);
		//TODO
		break;
	case 'i': //int in dezimal
		i = va_arg(*ap, int);
		//TODO print_integer(i, base);
		uart_write(i+48);
		break;
	case 'u': //unsigned int in dezimal
		//TODO 
		//TODO print_integer(i, 10);
		break;
	case 'p': // pointer in hex mit prefix 0x
		//TODO
		break;
	case '%': // einfaches %
		uart_write(37);
		break;
	}
	//va_end(ap);
}

void kprintf(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	
	while(*fmt){
		switch(*fmt){
		case '%':
			if(fmt+1){
				fmt++;
				replace_and_write(fmt, &ap);
				fmt++;
			}
			break;
		default:
			uart_write(*fmt);
			fmt++;
			break;
		}
	}
	va_end(ap);
}
