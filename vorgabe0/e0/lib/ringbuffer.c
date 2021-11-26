#include <kernel/kprintf.h>
#include <config.h>

struct input_buffer {
  unsigned char data[UART_INPUT_BUFFER_SIZE];
  unsigned int count; 	
  unsigned int read; 	
  unsigned int write; 	
};

static volatile struct input_buffer * const buffer;


void initialize_buffer(struct input_buffer *buffer){
	buffer->count = 0;
	buffer->read = 0;
	buffer->write = 0;
}

void buffer_push(unsigned char *input_byte, struct input_buffer *buffer){

	buffer->data[buffer->write] = *input_byte;
	if(buffer->count < UART_INPUT_BUFFER_SIZE){
		buffer->count ++;
		kprintf("buffer count after push: %i/%i", buffer->count, UART_INPUT_BUFFER_SIZE);
	}
	if(buffer->write == UART_INPUT_BUFFER_SIZE - 1){
		buffer->write = 0; //start from 0 when at the end
		if(buffer->read == UART_INPUT_BUFFER_SIZE - 1){
			buffer->read = 0; //start from 0 when at the end
		}
	}else{
		buffer->write ++;
		if(buffer->write == buffer->read + 1 
		   && buffer->count == UART_INPUT_BUFFER_SIZE){
			buffer->read ++;
		}
	}
	
	
}

unsigned char buffer_pull(struct input_buffer *buffer){
	if(buffer->count > 0){
		return buffer->data[buffer->read];
		buffer->count --;
		kprintf("buffer count after pull: %i/%i", buffer->count, UART_INPUT_BUFFER_SIZE);
		if(buffer->read == UART_INPUT_BUFFER_SIZE - 1){
			buffer->read = 0; //start from 0 when at the end
		}else{
			buffer->read ++;
		}
	}else{
		kprintf("cant pull: input_buffer empty!\n");
	}
}
