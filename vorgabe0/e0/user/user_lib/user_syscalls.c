#include<stdarg.h>
#include <kernel/kprintf.h>







void syscall_put_char(unsigned char c);

unsigned char syscall_get_char(void);

void syscall_kill_thread();

void syscall_create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(), unsigned int regs[]);

void syscall_sleep_thread();

void printf(char *fmt, ...);
