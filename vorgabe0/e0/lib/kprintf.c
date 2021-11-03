#include<stdarg.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>


void print_integer(int i, int base, char *c){

	const char *symbols = "0123456789abcdef";
	char output_buffer[10];
	//if(*c != 'x'){
		for(int j = 0; j < 10; j++){
			output_buffer[j] = *c;
		}
	//}
	int slot = 0;
	
	if(i == 0){
		uart_write('0');
		return;
	}
	
	while(i > 0){
		output_buffer[slot] = symbols[(i%base)];
		slot++;
		
		i = i / base;
	}
	if((slot < 8) && (*c != 'x')){
		slot = 7;
	}
	while(slot > -1){
		uart_write(output_buffer[slot]);
		slot--;
	}
	return;
}


void replace_and_write(char *fmt, va_list *ap, char *c){
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
		print_integer(u_int, 16, c);
		break;
		
	case 'i': //int in dezimal
		i = va_arg(*ap, int);
		if(i < 0){
			kprintf("-");						//TODO Vorzeichen integrieren
			print_integer(i*-1, 10, c);
		}else{
			print_integer(i, 10, c);
		}
		break;
		
	case 'u': //unsigned int in dezimal
		u_int = va_arg(*ap, unsigned int);
		print_integer(u_int, 10, c);
		break;
		
	case 'p': // pointer in hex mit prefix 0x
		//TODO
		break;
		
	case '%': // einfaches %
		uart_write(37);
		break;
		
	case '0': // padding with 0
		fmt = fmt + 2;
		replace_and_write(fmt, ap, "0");
		break;
		
	case '8': // field width
		fmt = fmt + 1;
		replace_and_write(fmt, ap, " ");
		break;
	}
	va_end(*ap);
	return;
}

void kprintf(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	
	while(*fmt){
		switch(*fmt){
		case '%':
			if(fmt+1){
				fmt++;
				if(*fmt == '8'){ 
					replace_and_write(fmt, &ap, "x");
					fmt = fmt + 2;
				}else if(*fmt == '0'){ 
					replace_and_write(fmt, &ap, "x");
					fmt = fmt + 3;
				}else{
					replace_and_write(fmt, &ap, "x");
				}
				//fmt = fmt + 2;
				
			}
			break;
		default:
			uart_write(*fmt);
			fmt++;
			break;
		}
	}
	va_end(ap);
	return;
}
