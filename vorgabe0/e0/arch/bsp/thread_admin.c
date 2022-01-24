#include <lib/unterprogramm.h>
#include <lib/kmemcpy.h>
#include <kernel/kprintf.h>
#include <kernel/idle_thread.h>
#include <lib/ringbuffer.h>
#include <arch/bsp/thread_admin.h>

#define LR		21
#define SP		19
#define CPSR	17
#define USER_MODE 0x10

#define THREAD_COUNT	32
#define USER_STACK_BASE 0x27000
#define USER_STACK_SIZE 0x1000

/*	Layout vom regs[35] Array:
	regs[34-22]		R12-R0
	regs[21]		LR
	regs[20]		PC
	regs[19]		SP
	regs[18-0]		Daten für den Registerprint		
*/


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/  DATA /_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
/*
struct tcb{
	//thread's context
	unsigned int id;
	unsigned int in_use;
	unsigned int pc;
	unsigned int sp;
	unsigned int cpsr;
	unsigned int registers[13];
	
	//DEBUG
	unsigned char data;
};
*/
struct tcb tcbs[THREAD_COUNT];
struct tcb *free_tcb; //first free tcb slot
unsigned int used_tcbs;
/*

struct list_elem{
	struct list_elem *next;
	struct list_elem *prev;
	struct tcb *context;
	unsigned int sleep_time;
};
struct list{
	struct list_elem *curr;
	struct list_elem *last;
	unsigned int count;
	//struct list_elem * elements[32];
};
*/
struct list r_queue;
struct list *ready_queue = &r_queue;
struct list w_queue;
struct list *waiting_queue = &w_queue;

struct list_elem threads[THREAD_COUNT];


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/ DEBUG /_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void print_list_elem(unsigned int _j){
	if(ready_queue->curr->context->id == _j){ 
		kprintf("\t\t\t  curr->");} else {kprintf("\t\t\t\t");}
		
	//kprintf("[%i]: '%c'", _j, *(unsigned char*)(tcbs[_j].registers[0]));
	kprintf("[%i]'%c'", _j, (tcbs[_j].data));
	
	if(ready_queue->last->context->id == _j){ 
		kprintf("<-last\n");} else {kprintf("\n");}
}	

void print_ready_queue(){
	//return;
	kprintf("\n");
	//kprintf("queue has %i active threads:\n", used_tcbs);
	//kprintf("free_tcb at: tcb[%i]\n", free_tcb->id);
	if(ready_queue->curr == 0x0) kprintf("  \t\t\tcurr->0x0\n");
	unsigned int j = ready_queue->curr->context->id;
	for(unsigned int i = 0; i < used_tcbs; i++){
		print_list_elem(j);
		j = threads[j].next->context->id;
	}
	kprintf("\n");
}


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/ SETUP /_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void init_ready_queue(){
	ready_queue->curr = 0x0;
	ready_queue->last = 0x0;
	ready_queue->count = 0;
}

void init_waiting_queue(){
	waiting_queue->curr = 0x0;
	waiting_queue->last = 0x0;
	waiting_queue->count = 0;
}

void init_all_tcbs(){
	for(unsigned int i = 0; i < THREAD_COUNT; i++){
		tcbs[i].sp = USER_STACK_BASE + (i * USER_STACK_SIZE);
		tcbs[i].id = i;
		tcbs[i].in_use = 0;
	}
	
	free_tcb = &tcbs[0];
	used_tcbs = 0;
}
void init_thread_slots(){
	for(int i = 0; i < THREAD_COUNT; i++){
			threads[i].context = &tcbs[i];
	}
}

void init_thread_admin(){
	kprintf("INIT THREAD ADMIN\n");
	init_ready_queue();
	init_waiting_queue();
	kprintf("now ready_queue->curr: %p\n", ready_queue->curr);
	kprintf("now waiting_queue->curr: %p\n", waiting_queue->curr);
	init_all_tcbs();
	init_thread_slots();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/ SCHEDULER /_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void save_context(unsigned int regs[]){
	(ready_queue->curr->context->pc) = regs[LR];
	(ready_queue->curr->context->sp) = regs[SP];
	(ready_queue->curr->context->cpsr) = regs[CPSR];
	kmemcpy(&(ready_queue->curr->context->registers), &regs[22], 13 * sizeof(unsigned int));
}

void load_context(unsigned int regs[], struct tcb* context){
	regs[LR] = context->pc;
	regs[SP] = context->sp;
	regs[CPSR] = context->cpsr;
	kmemcpy(&regs[22], &(context->registers), 13 * sizeof(unsigned int));
	
}

void change_context(unsigned int regs[]){
	save_context(regs);
	load_context(regs, ready_queue->curr->next->context);
	//change pointer positions
	ready_queue->curr = ready_queue->curr->next;
	ready_queue->last = ready_queue->curr->prev;
}

void scheduler(unsigned int regs[]){
	//kprintf("\nscheduling %i threads\n", used_tcbs);
	if(used_tcbs > 1){
		change_context(regs);
		kprintf("\n");
		
		//DEBUG
		print_ready_queue();
	}else if(ready_queue->curr->context->data == '#'){ // # = masterprogramm
		load_context(regs, ready_queue->curr->next->context);
	}
	
	
	//kprintf("active threads: %i\n", used_tcbs);
	return;
}


void check_for_waiting_threads(unsigned int regs[35]){
	kprintf("checking waiting threads, waiting_queue->count: %i\n", waiting_queue->count);
	if(waiting_queue->count == 0){
		kprintf("no waiting thread!\n");
		return;
	}
	
	struct list_elem *_temp = waiting_queue->curr;
	for(unsigned int i = 0; i < waiting_queue->count; i++){
		if(_temp->sleep_time == 0){
			kprintf("found waiting thread: thread[%i]\n", _temp->context->id);
			wake_thread(buffer_pull(&uart_input_buffer), _temp, regs);
			return;
		}
		_temp = _temp->next;
	}
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/ THREAD ADMINISTRATION /_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
int find_free_tcb(){
	for(int i = 0; i < THREAD_COUNT; i++){
		if(tcbs[i].in_use == 0){
			free_tcb = &tcbs[i];
			return 1;
		}
	}
	free_tcb = 0x0;
	return 0;
}

void decrease_sp(unsigned int* _sp, unsigned int size){
	if(size < 8){
		*_sp -= 8;
		
	}else{
		*_sp -= size;
		*_sp -= (size - (size % 8));
	}
}

unsigned int fill_tcb(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*)){

	free_tcb->pc = (unsigned int) unterprogramm;
	free_tcb->cpsr = USER_MODE;
	
	free_tcb->sp = USER_STACK_BASE + (free_tcb->id * USER_STACK_SIZE);
	decrease_sp(&free_tcb->sp, (count * sizeof(unsigned char*)));
	kmemcpy((void*)free_tcb->sp, data, count * sizeof(unsigned char*));
	
	free_tcb->registers[0] = free_tcb->sp;
	
	//DEBUG
	free_tcb->data = *data;
	
	//kprintf("fill $r0 of tcb[%i]: sp->%c\n", free_tcb->id, *(unsigned char*)free_tcb->registers[0]);
	free_tcb->in_use = 1;
	return free_tcb->id;
}


void push_tcb_to_ready_queue(unsigned int thread_id, unsigned int irq_regs[]){	
	//no threads active
	if(used_tcbs == 0){ 							
		ready_queue->curr = &threads[thread_id];
		ready_queue->last = &threads[thread_id];
		ready_queue->curr->next = ready_queue->curr;
		ready_queue->curr->prev = ready_queue->curr;
		load_context(irq_regs, ready_queue->curr->context);
		
	
	//exactly 1 thread active
	} else if(used_tcbs == 1){ 	
		ready_queue->curr->next = &threads[thread_id];
		ready_queue->curr->prev = &threads[thread_id];
		threads[thread_id].next = ready_queue->curr;
		threads[thread_id].prev = ready_queue->curr;
		ready_queue->last = &threads[thread_id];
	
	//2 or more threads active
	} else{
		ready_queue->last->next = &threads[thread_id];
		threads[thread_id].prev = ready_queue->last;
		threads[thread_id].next = ready_queue->curr;
		ready_queue->curr->prev = &threads[thread_id];
		ready_queue->last = &threads[thread_id];
	}
	
	ready_queue->count ++;
	threads[thread_id].sleep_time = 0;
}


void create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*), unsigned int irq_regs[]){
	if(!find_free_tcb()){
		kprintf("cant create thread! already %i threads running..\n", THREAD_COUNT);
		return;}
		
	//DEBUG
	//print_ready_queue();
	//kprintf("\ncreating threads[%i] with char: %c\n", free_tcb->id, *data);
	
	unsigned int thread_id = fill_tcb(data, count, unterprogramm);
	push_tcb_to_ready_queue(thread_id, irq_regs);
	used_tcbs ++;
	find_free_tcb();
	
	//DEBUG
	print_ready_queue();
	//kprintf("leaving create thread..\n");
}

void kill_thread(unsigned int regs[]){
	//killing the only thread
	if(used_tcbs == 1){
		//kprintf("\nkilling last thread [%i]'%c'\n", ready_queue->curr->context->id, ready_queue->curr->context->data);
		ready_queue->curr->context->in_use = 0;
		ready_queue->curr = 0x0;
		regs[LR] = (unsigned int) &idle_thread;	
		
		//DEBUG
		//print_ready_queue();
		
	}else if(used_tcbs > 1){
		//kprintf("\nkilling [%i]'%c'\n", ready_queue->curr->context->id, ready_queue->curr->context->data);
		ready_queue->curr->context->in_use = 0;
		ready_queue->curr->next->prev = ready_queue->curr->prev;
		ready_queue->curr->prev->next = ready_queue->curr->next;
		ready_queue->curr = ready_queue->curr->next;
		load_context(regs, ready_queue->curr->context);
	}
	used_tcbs--;
	ready_queue->count--;
	
	
	//DEBUG
	print_ready_queue();
	
	return;
}

void wait_thread(unsigned int sleep_time, unsigned int regs[]){
	
	kprintf("thread_admin speaking: going to make thread[%i] wait for %i..\n", ready_queue->curr->context->id, sleep_time);
	ready_queue->curr->sleep_time = sleep_time; // (sleep_time ==0 && thread is in waiting_queue) -> thread is waiting for char
												// (sleep_time  >0 && thread is in waiting_queue) -> thread is sleeping
	struct list_elem *temp_curr = ready_queue->curr;
	//TODO 
	kprintf("before waiting_queue->count: %i\n", waiting_queue->count);
	kprintf("before ready_queue->count: %i\n", ready_queue->count);
	if(waiting_queue->count == 0){
		//verschiebe ready_queue->curr zu waiting_queue->curr
		waiting_queue->curr = ready_queue->curr;
		waiting_queue->last = ready_queue->curr;
		waiting_queue->curr->next = waiting_queue->curr;
		waiting_queue->curr->prev = waiting_queue->curr;
		
	} else if(waiting_queue->count == 1){ 	
		waiting_queue->curr->next = ready_queue->curr;
		waiting_queue->curr->prev = ready_queue->curr;
		ready_queue->curr->next = waiting_queue->curr;
		ready_queue->curr->prev = waiting_queue->curr;
		waiting_queue->last = ready_queue->curr;
		
	}else{
		waiting_queue->last->next = ready_queue->curr;
		ready_queue->curr->prev = waiting_queue->last;
		ready_queue->curr->next = waiting_queue->curr;
		waiting_queue->curr->prev = ready_queue->curr;
		waiting_queue->last = ready_queue->curr;
	}
	
	//ready_queue neu sortieren
	if(ready_queue->count == 1){
		ready_queue->curr = 0x0;
		regs[LR] = (unsigned int) &idle_thread;
		
	} else if(ready_queue->count > 1){
		temp_curr->next->prev = temp_curr->prev;
		temp_curr->prev->next = temp_curr->next;
		ready_queue->curr = temp_curr->next;
		load_context(regs, ready_queue->curr->context);
	}
	
	ready_queue->count--;
	waiting_queue->count++;
	kprintf("now waiting_queue->count: %i\n", waiting_queue->count);
	kprintf("now ready_queue->count: %i\n", ready_queue->count);
}


void wake_thread(unsigned char _send_char, struct list_elem* _wait_thread, unsigned int regs[35]){
	//_wait_thread bekommt _send_char in $r0
	kprintf("im going to wake thread[%i] with char: %c\n", _wait_thread->context->id, _send_char);
	_wait_thread->context->registers[0] = (unsigned int) _send_char;
	
	//_wait_thread in ready_queue einordnen
	unsigned int _thread_id = _wait_thread->context->id;
	push_tcb_to_ready_queue(_thread_id, regs);
	
	//_wait_thread aus waiting_queue löschen
	if(waiting_queue->count == 1){
		waiting_queue->curr = 0x0;
	} else if(waiting_queue->count > 1){
		_wait_thread->next->prev = _wait_thread->prev;
		_wait_thread->prev->next = _wait_thread->next;
		waiting_queue->curr = waiting_queue->curr->next;
	}

	ready_queue->count++;
	waiting_queue->count--;
}





