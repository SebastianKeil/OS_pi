#include <arch/bsp/yellow_led.h>

#define UART_BASE (0x7E201000 - 0x3F000000)

struct uart {
	//0x0
	unsigned int dr;			//data reg
	unsigned int rsrecr;		//receive status register/error clear reg
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
	unsigned int dmacr; 		//masked interrupt status reg
	unsigned int unused2[13];
	//0x80
	unsigned int itcr;			//test control reg
	unsigned int itip;			//integration test input reg
	unsigned int itop;			//integration test output reg
	unsigned int tdr;			//test data reg
};

static volatile
struct gpio * const gpio_port = (struct gpio *)GPIO_BASE;

void yellow_on(void)
{
	/* Initialisieren */
	gpio_port->func[0] = gpio_output << (YELLOW_LED * GPF_BITS);

	/* Anschalten */
	gpio_port->set[0] = 1 << YELLOW_LED;
}

