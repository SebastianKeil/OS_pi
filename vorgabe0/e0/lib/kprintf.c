#include<stdarg.h>
#include <lib/kprintf.h>
#include <arch/bsp/uart.h>


char* prepare_for_print(char *fmt, va_list *ap){
	if(*fmt == '8'){ 
		fmt++;
		if(*fmt == 'i' || *fmt == 'u' || *fmt == 'x' || *fmt == 'p'){	
			replace_and_write(fmt, ap, " ");
			fmt++;
			return(fmt);
		}else{
			kprintf("\nERROR conversion with %c not available\n", *fmt); return(fmt+1);
		}
		
	}else if(*fmt == '0'){ 
		fmt++;
		if(*fmt == '8'){
			fmt++;
			if(*fmt == 'i' || *fmt == 'u' || *fmt == 'x'){	
				replace_and_write(fmt, ap, "0");
				fmt++;
				return(fmt);
			}else{
			kprintf("\nERROR conversion with %c not available\n", *fmt); return(fmt+1);
			}
		}else{
			kprintf("\nERROR padding only available with eight zeros\n"); return(fmt+1);
		}
	}else if(*fmt == 'c' || *fmt == 's' || *fmt == 'x' || *fmt == 'i' || *fmt == 'u' || *fmt == 'p' || *fmt == '%'){	
				replace_and_write(fmt, ap, "x");
				fmt++;
				return(fmt);
	}else{
		kprintf("\nERROR unknown format: %c\n", *fmt); return(fmt+1);
	}
return(fmt);
}


void print_integer(unsigned int i, int base, char *c, int prefix, int neg){
	const char *symbols = "0123456789abcdef";
	const char *pre = "0x";
	const char *nega = "-";
	char output_buffer[12];
	if(*c != 'x'){
		for(int j = 0; j < 11; j++){
			output_buffer[j] = *c;
		}
	}

	int slot = 0; //position in Buffer
	if (i == 0){
		output_buffer[slot] = symbols[0];
		slot++;
	}
	while(i > 0){
		output_buffer[slot] = symbols[(i%base)];
		slot++;
		i = i / base;
	}
	if(prefix){
		output_buffer[slot] = pre[1];
		slot++;
		output_buffer[slot] = pre[0];
		slot++;
	}
	if(neg){
		if(*c == '0'){
			if(slot < 8){
				output_buffer[7] = nega[0];
			} else {
				output_buffer[slot] = nega[0];
			}
		} else {
			output_buffer[slot] = nega[0];
		}
		slot++;
	}
	slot--;
	if((slot < (8)) && (*c != 'x')){
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
	const char *string;		//TODO change name of string to something more descriptive
	unsigned int u_int;
	switch(*fmt){
	case 'c': //int -> unsigned char
		i = va_arg(*ap, int);
		uart_write(i);
		break;
		
	case 's': //whole string
		string = va_arg(*ap, const char *);
		while(*string){
			uart_write(*string);
			string ++;
		}
		break;
		
	case 'x': //unsigned int -> hex
		u_int = va_arg(*ap, unsigned int);
		print_integer(u_int, 16, c, 0, 0);
		break;
		
	case 'i': //signed int -> decimal
		i = va_arg(*ap, int);
		if(i < 0){
			print_integer(i, 10, c, 0, 1);
		}else{
			print_integer(i, 10, c, 0, 0);
		}
		break;
		
	case 'u': //unsigned int -> decimal
		u_int = va_arg(*ap, unsigned int);
		print_integer(u_int, 10, c, 0, 0);
		break;
		
	case 'p': // pointer -> hex with prefix 0x
		u_int = va_arg(*ap, unsigned int);
		if(*c == 'x'){
			uart_write(48);
			uart_write(120);
			print_integer(u_int, 16, c, 0, 0);
		} else {
			print_integer(u_int, 16, c, 2, 0);
			}
		break;
		
	case '%': // single %
		uart_write(37);
		break;
	}
	//va_end(*ap);
	return;
}


void kprintf(char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	while(*fmt){
		switch(*fmt){
		case '%':
			fmt++;
			fmt = prepare_for_print(fmt, &ap);
			break;
		
		case '\\':
			fmt++;
			if(*fmt == 'n'){
				uart_write(10);
				fmt ++;
				break;
			} else {
				uart_write(92);
				break;
			}
			
		default:
			uart_write(*fmt);
			fmt++;
			break;
		}
	}
	va_end(ap);
	return;
}
