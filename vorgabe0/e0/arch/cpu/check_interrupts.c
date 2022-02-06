#include <kernel/kprintf.h>
#include <lib/regcheck.h>
#include <arch/cpu/shared.h>
#include <kernel/thread_admin.h>

//int print_register_dump;
//unsigned char receive_buffer;

void check_interrupts(unsigned char data){
	unsigned char receive_buffer = data;
	switch(receive_buffer){

		case 'N':		//lesender Zugriff auf Null-Pointer
			asm volatile 	("ldr r0, =0x0\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");		 
			break;
		case 'P': 			//Sprung auf Null-Pointer
			asm volatile 	("ldr pc, =0x0");
			break;

		case 'C':			//schreibender Zugriff auf eigenen Code
			asm volatile 	("ldr r0, =0x100020\t\n"
							"str r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'U':			//lesender Zugriff auf nicht zugeordnete Adresse
			asm volatile 	("ldr r0, =0x5a00000\t\n"
							"ldr r0, [r0]\t\n"
							: : : "r0");
			break;

		case 'X':			//Sprung auf eigene Daten oder Stack
			asm volatile 	("ldr pc, =0x300000");
			break;
			
		default:
			break;
	}
}
