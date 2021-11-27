#ifndef UART_H
#define UART_H

void set_uart_receive_interrupt();
void disable_uart_fifo(void);
unsigned char uart_read(void);
void uart_write(char data);

#endif
