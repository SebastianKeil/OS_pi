#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H

#include<stdarg.h>

/*
syscall_put_char(unsigned char c)	->	asm volatile("svc #42");		
syscall_get_char()					->	asm volatile("svc #43");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #44");
syscall_sleep_thread()				->	asm volatile("svc #45");
*/

void syscall_put_char(unsigned char c);
unsigned char syscall_get_char(void);
void syscall_kill_thread();
void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*));
void syscall_ufork(void (*programm)(void));
void syscall_sleep_thread();

#endif
