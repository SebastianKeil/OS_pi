#include <config.h>
#include <user/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>


unsigned char input_svc;

void masterprogramm(unsigned char *void_char){	

	while(1){
		input_svc = syscall_get_char();
		syscall_create_thread(&input_svc, 1, &unterprogramm);
	}	
}



