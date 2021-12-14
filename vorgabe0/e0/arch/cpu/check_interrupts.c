#include <kernel/kprintf.h>
#include <lib/regcheck.h>
#include <arch/cpu/shared.h>
#include <lib/unterprogramm.h>
#include <arch/bsp/thread_admin.h>

int print_register_dump;
extern void (*unterprogramm)();

void check_for_interrupts(char receive_buffer){
	switch(receive_buffer){
		case 'S':
			//supervisor call
			kprintf("test: supervisor interrupt ?from kernel?\n");
			asm volatile("svc 0");
					 
			break;
		case 'A':
			//data abort
			kprintf("test: data interrupt from kernel\n");
			asm volatile("bkpt #0");
			break;
		case 'U':
			//undefined instruction
			kprintf("test: undefined interrupt from kernel\n");
			asm volatile("udf");
			break;
		case 'P':
			//prefetch abort
			kprintf("test: prefetch interrupt from kernel\n");
			asm volatile("bkpt #1");
			break;
		case 'D':
			//register dump on-off-switch
			//set global variable 
			if(print_register_dump == 0){ 
				print_register_dump = 1;
				kprintf("register dump on\n");
			}else{ 
				print_register_dump = 0;
				kprintf("register dump off\n");
			}
			break;
			/*???????
		case 'c':
			//register checker ausführen
			kprintf("test: register checker\n");
			register_checker();
			break;
			*/
			
			case 't':
			//data abort
			kprintf("testcase for create_thread()\n");
			create_thread(unterprogramm; receive_buffer);
			break;
			
			//TODO: more cases!
			
		default:
			create_thread(unterprogramm; receive_buffer);
	}
}
