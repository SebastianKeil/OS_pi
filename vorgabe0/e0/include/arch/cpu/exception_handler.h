#include <lib/utilities.h>
#include <kernel/kprintf.h>
#include <arch/bsp/interrupt_ctrl.h>
#include <arch/cpu/shared.h>
#include <arch/bsp/uart.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/sys_timer.h>

#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

//reset
#define UND 1
#define SVC 2
#define PRE 3
#define DATA_ABORT 4
// unused 5
#define IRQ 6
#define FIQ 7

#define SYS_TIMER 2
#define UART_INT 25

void undefined_instruction(unsigned int regs[35]);
void software_interrupt(unsigned int regs[35]);
void prefetch_abort(unsigned int regs[35]);
void data_abort(unsigned int regs[35]);
void fiq(unsigned int regs[35]);
void irq(unsigned int regs[35]);

#endif
