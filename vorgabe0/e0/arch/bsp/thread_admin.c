

struct thread_stack{
	unsigned int stack[1024]; //4096 bytes
}

struct thread_stacks[32];



struct tcb{
	//thread's context
	unsigned int pc;
	unsigned int sp;
	unsigned int cpsr;
	unsigned int registers[13];
}

struct tcb tcbs[33]; //32 threads supported
struct tcb * free = tcbs[0]; //first free tcb slot



struct list_elem{
	struct list_elem * next;
	struct list_elem * prev;
	struct tcb * context;
}

struct list{
	struct list_elem * first;
	struct list_elem * free; //first free element
	//struct list_elem * elements[32];
}

init_tcbs(){
	struct tcb tcb1;
	
}

void init_tcb(unsigned char data){
	
}

void create_thread(unsigned char receive_buffer, ){
	init_tcb();
	push_to_queue();
	
}
