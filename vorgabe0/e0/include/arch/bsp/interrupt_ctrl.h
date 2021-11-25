#ifndef INTERRUPT_CTRL_H
#define INTERRUPT_CTRL_H

void enable_interrupts_timer(void);
void enable_interrupts_uart(void);
void get_pending_status(int *sys_timer_pending, int *uart_pending);

#endif
