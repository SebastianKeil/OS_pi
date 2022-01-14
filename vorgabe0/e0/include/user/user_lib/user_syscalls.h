#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H

#include<stdarg.h>

/*
syscall_put_char()					->	asm volatile("svc #1");		
syscall_get_char()					->	asm volatile("svc #2");
syscall_kill_thread()				->	asm volatile("svc #69");
syscall_create_thread()				->	asm volatile("svc #4");
syscall_sleep_thread()				->	asm volatile("svc #5");
printf(char* fmt_str, unsigned int)	->  kprintf();
*/

void syscall_put_char(unsigned char c);
unsigned char syscall_get_char(void);
void syscall_kill_thread();
void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(), unsigned int regs[]);
void syscall_sleep_thread();
void printf(char *fmt, ...);

