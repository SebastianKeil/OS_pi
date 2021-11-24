#ifndef UART_H
#define UART_H

void disable_uart_fifo(void);
char uart_read(void);
void uart_write(char data);

#endif
