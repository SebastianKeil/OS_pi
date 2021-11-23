#ifndef KPRINTF_H
#define KPRINTF_H

#include<stdarg.h>

char* prepare_for_print(char *fmt, va_list *ap);
void print_integer(unsigned int i, int base, char *c, int prefix, int neg);

void replace_and_write(char *fmt_next, va_list *ap_next, char *c);
void kprintf(char *fmt, ...);

#endif
