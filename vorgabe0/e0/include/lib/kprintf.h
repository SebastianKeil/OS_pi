#ifndef KPRINTF_H
#define KPRINTF_H

#include<stdarg.h>

void print_integer(int i, int base, char *c);
void replace_and_write(char *fmt_next, va_list *ap_next, char *c);
void kprintf(char *fmt, ...);

#endif
