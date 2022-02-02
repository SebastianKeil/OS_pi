#include <kernel/kprintf.h>
#include <config.h>

struct input_buffer {
  unsigned char data[UART_INPUT_BUFFER_SIZE];
  unsigned int count; 	
  unsigned int read; 	
  unsigned int write; 	
};

struct input_buffer uart_input_buffer;
unsigned char pulled_char;


void initialize_buffer(struct input_buffer *buffer){
	buffer->count = 0;
	buffer->read = 0;
	buffer->write = 0;
	//kprintf("UART_INPUT_BUFFER INITIALIZED\n");
}

unsigned int get_buffer_count(struct input_buffer *buffer){
	return buffer->count;
}

void buffer_push(unsigned char input_byte, struct input_buffer *buffer){

	buffer->data[buffer->write] = input_byte;
	if(buffer->count < UART_INPUT_BUFFER_SIZE){
		buffer->count ++;
	}
	
	//kprintf("\n************* RINGBUFFER MESSAGE  *****************\n");
	//kprintf("buffer count after pushing '%c': %i/%i\n", input_byte, buffer->count, UART_INPUT_BUFFER_SIZE);
	
	if(buffer->write == UART_INPUT_BUFFER_SIZE - 1){
		buffer->write = 0; //start from 0 when at the end
		if(buffer->read == UART_INPUT_BUFFER_SIZE - 1 && buffer->count == UART_INPUT_BUFFER_SIZE){
			buffer->read = 0; //start from 0 when at the end
		}
	}else{
		buffer->write ++;
		if(buffer->write == buffer->read + 1 
		   && buffer->count == UART_INPUT_BUFFER_SIZE){
			buffer->read ++;
		}
	}
	//kprintf("read is on: %i\n", buffer->read);
	//kprintf("write is on: %i\n", buffer->write);
	//kprintf("***************************************************\n");
	
}

unsigned char buffer_pull(struct input_buffer *buffer){
	if(buffer->count == 0){
		kprintf("cant pull: input_buffer empty!\n");
		return '0';
	}
	
	pulled_char = buffer->data[buffer->read];
	buffer->count --;
	if(buffer->count == 0){ 
		buffer->read ++;
		return pulled_char;
	}
	
	//kprintf("buffer count after pull: %i/%i\n", buffer->count, UART_INPUT_BUFFER_SIZE);
	
	if(buffer->read == UART_INPUT_BUFFER_SIZE - 1){
		buffer->read = 0; //start from 0 when at the end
	}else{
		buffer->read ++;
	}
	
	return pulled_char;
}
