#include <config.h>
#include <user/unterprogramm.h>
#include <user/user_lib/user_syscalls.h>
#include <user/masterprogramm.h>


unsigned char input_svc;
unsigned char dummy_char;

void create_masterprogram_thread(){
	dummy_char = '#';
	syscall_create_thread(&dummy_char, 1, &masterprogramm);
}


void masterprogramm(unsigned char *void_char){	
	while(1){
		input_svc = syscall_get_char();
		syscall_create_thread(&input_svc, 1, &unterprogramm);
	}	
}



