#include <arch/bsp/yellow_led.h>
#include <kernel/kprintf.h>

#define UART_BASE (0x7E201000 - 0x3F000000)
#define RXFE 4
#define TXFE 5
#define FEN 4
#define RXIM 4
#define RTIC 6

struct uart {
	//0x0
	unsigned int dr;			//data reg
	unsigned int rsrecr;			//RX status reg/error clear reg
	unsigned int unused0[4];
	unsigned int fr;			//flag reg
	unsigned int unused1;
	//0x20
	unsigned int ilpr;			//disabled IrDA reg
	unsigned int ibrd;			//integer boud rate divisor
	unsigned int fbrd;			//fractional boud rate divisor
	unsigned int lcrh;			//line control reg
	//0x30
	unsigned int cr;			//control reg
	unsigned int ifls;			//interrupt FIFO level select reg
	unsigned int imsc;			//interrupt mask set clear reg
	unsigned int ris;			//raw interrupt status reg
	//0x40
	unsigned int mis; 			//masked interrupt status reg
	unsigned int icr; 			//interrupt clear reg
	unsigned int dmacr; 			//masked interrupt status reg
	unsigned int unused2[13];
	//0x80
	unsigned int itcr;			//test control reg
	unsigned int itip;			//integration test input reg
	unsigned int itop;			//integration test output reg
	unsigned int tdr;			//test data reg
};

static volatile struct uart * const uart_port = (struct uart *)UART_BASE;
/* 
   fr: flag reg
   3 BUSY ..... the UART is busy transmitting data
-> 4 RXFE ..... receive FIFO is empty
-> 5 TXFF ..... transmit FIFO is full
   6 RXFF ..... receive FIFO is full
   7 TXFE ..... transmit FIFO is empty.
*/


void reset_uart_interrupt(){
	uart_port->icr |= (1 << RTIC);
	
	kprintf("UART_RECEIVE_INTERRUPT CLEARED\n");
	//kprintf("RTIC: %i\n", uart_port->icr & (1 << RTIC));
}

void set_uart_receive_interrupt(){
	unsigned int reg_copy = uart_port->imsc;
	reg_copy |= (1 << RXIM);
	uart_port->imsc = reg_copy;
	kprintf("UART RECEIVE INTERRUPT ENABLED\n");
}

void disable_uart_fifo(){
	unsigned int reg_copy = uart_port->lcrh;
	reg_copy &= ~(1UL << FEN);
	uart_port->lcrh = reg_copy;
	kprintf("UART FIFO DISABLED\n");
}

unsigned char uart_read(void){
	//while(uart_port->fr & (1 << RXFE)){}
	unsigned char data;
	data = (uart_port->dr & 255);
	return data;
}

void uart_write(char data){
	while(uart_port->fr & (1 << TXFE))
	{}
	uart_port->dr = data;
}




