#ifndef KPRINTF_H
#define KPRINTF_H

#include<stdarg.h>

void kprintf(char *fmt, ...);
void replace_and_write(char *fmt_next, va_list *ap_next);

#endif
