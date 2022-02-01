#include <kernel/kprintf.h>
#include <config.h>
#include <lib/regcheck.h>
#include <user/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>


unsigned char input_svc;

void masterprogramm(unsigned char *void_char){	

	while(1){
		//kprintf("masterprogramm: \n\tasking for char\n");
		input_svc = syscall_get_char();
		//kprintf("masterprogramm: \n\treceived char %c\n", input_svc);
		syscall_create_thread(&input_svc, 1, &unterprogramm);
	}	
}



