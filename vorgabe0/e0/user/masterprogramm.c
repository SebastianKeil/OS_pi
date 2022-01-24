#include <kernel/kprintf.h>
#include <config.h>
#include <lib/regcheck.h>
#include <user/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>


unsigned char input_svc;

void masterprogramm(unsigned char *void_char){	
	kprintf("entering masterprogramm..\n");

	while(1){
		kprintf("now asking for char..\n");

		input_svc = syscall_get_char();

		kprintf("masterprogramm speaking: received char, giong to create unterprogramm!\n");
		syscall_create_thread(input_svc, 1, &unterprogramm);
	}	
}



