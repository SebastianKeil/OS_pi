#include <kernel/kprintf.h>
#include <config.h>
#include <lib/regcheck.h>
#include <user/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>




void masterprogramm(unsigned char *void_char){	
	kprintf("entering masterprogramm..\n");
	unsigned char input = syscall_get_char();
	
	//syscall_create_thread(input, 1, &unterprogramm);
}



