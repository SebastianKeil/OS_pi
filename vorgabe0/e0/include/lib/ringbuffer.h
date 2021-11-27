#include <config.h>

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

struct input_buffer {
  unsigned char data[UART_INPUT_BUFFER_SIZE];
  unsigned int count; 	
  unsigned int read; 	
  unsigned int write; 	
};

extern struct input_buffer * const buffer;

void initialize_buffer(struct input_buffer *buffer);
void buffer_push(unsigned char *input_byte, struct input_buffer *buffer);
unsigned char buffer_pull(struct input_buffer *buffer);

#endif
