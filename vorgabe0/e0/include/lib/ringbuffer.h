#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

void initialize_buffer(input_buffer *buffer);
void buffer_push(unsigned char *input_byte, input_buffer *buffer);
unsigned char buffer_pull(input_buffer *buffer);

#endif
