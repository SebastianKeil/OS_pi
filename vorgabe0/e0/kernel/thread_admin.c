#include <lib/unterprogramm.h>
#include <lib/kmemcpy.h>
#include <kernel/kprintf.h>
#include <kernel/idle_thread.h>
#include <lib/ringbuffer.h>
#include <kernel/thread_admin.h>

#define LR		21
#define SP		15
#define CPSR	17
#define USER_MODE 0x10

#define THREAD_COUNT	32
#define USER_STACK_BASE 0x7FEFF8
#define USER_STACK_SIZE 0x1000

/*	Layout vom regs[35] Array: R0=regs[22]
	regs[34-22]		R12-R0
	regs[21]		LR
	regs[15]		SP
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
//_/_/_/_/_/_/_/ DEBUG /_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void print_list_elem(unsigned int _j, struct list* queue){
	if(queue->curr->context->id == _j){ 
		kprintf("\t\t\t  curr->");} else {kprintf("\t\t\t\t");}
		
	//kprintf("[%i]: '%c'", _j, (unsigned char)(tcbs[_j].registers[0]));
	kprintf("[%i]'%c'", _j, (tcbs[_j].data));
	
	if(queue == waiting_queue){
		kprintf("(%i)", threads[_j].sleep_time);
	}
	
	if(ready_queue->last->context->id == _j){ 
		kprintf("<-last\n");} else {kprintf("\n");}
}	

void print_ready_queue(){
	//return;
	kprintf("\n\n*****************************************************");
	kprintf("\n\t\tready:\n", ready_queue->count);
	//kprintf("queue has %i active threads:\n", used_tcbs);
	//kprintf("free_tcb at: tcb[%i]\n", free_tcb->id);
	if(ready_queue->curr == 0x0){
		//kprintf("  \t\t\tcurr->0x0\n");
		kprintf("  \t\t\t/\n");
	}
	else {
		unsigned int j = ready_queue->curr->context->id;
		for(unsigned int i = 0; i < ready_queue->count; i++){
			print_list_elem(j, ready_queue);
			j = threads[j].next->context->id;
		}
	}
	kprintf("\n");
}

void print_waiting_queue(){
	//return;
	kprintf("\n\t\twaiting:\n", waiting_queue->count);
	//kprintf("queue has %i active threads:\n", used_tcbs);
	//kprintf("free_tcb at: tcb[%i]\n", free_tcb->id);
	if(waiting_queue->curr == 0x0){
		//kprintf("  \t\t\tcurr->0x0\n");
		kprintf("  \t\t\t/\n");
		}
	else {
		unsigned int j = waiting_queue->curr->context->id;
		for(unsigned int i = 0; i < waiting_queue->count; i++){
			print_list_elem(j, waiting_queue);
			j = threads[j].next->context->id;
		}
	}
	kprintf("*****************************************************\n\n");
	kprintf("\n");
}


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/ SETUP /_/_/_/_/_/_/_/_/_/_/
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
		tcbs[i].sp = USER_STACK_BASE + (i * USER_STACK_SIZE); //BASE: 0x27000 SIZE: 0x1000 -> thread[0]@27000, thread[1]@28000
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
	kprintf("#save_context: \n\tsaved thread[%i] \n\tstackpointer is at %p\n", ready_queue->curr->context->id, ready_queue->curr->context->sp);
}


void load_context(unsigned int regs[], struct tcb* context){

	kprintf("#load_context: \n\tloading thread[%i] \n\tstackpointer is at %p\n", context->id, context->sp);
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


unsigned int sleeping_threads[32];
unsigned int sleeping_threads_free = 0;

void update_sleeping_threads(unsigned int regs[]){

	struct list_elem *_temp = waiting_queue->curr;
	struct list_elem *_temp_next;
	unsigned int curr_waiting_count = waiting_queue->count;
	for(unsigned int i = 0; i < curr_waiting_count; i++){
		_temp_next = _temp->next;
		
		if(_temp->sleep_time > 1){
			_temp->sleep_time --;
		}else if(_temp->sleep_time == 1){
			wake_thread('#', _temp, regs);
		}
		
		_temp = _temp_next;
	}
}

void scheduler(unsigned int regs[]){

	if(ready_queue->count > 1){
		kprintf("#scheduler: \n\tgoing to schedul thread[%i] for thread[%i]\n", ready_queue->curr->next-> context->id, ready_queue->curr->context->id);
		change_context(regs);
		kprintf("\n");
		
		//DEBUG
		print_ready_queue();
	}
	
	update_sleeping_threads(regs);
	//print_waiting_queue();
	return;
}


void check_for_waiting_threads(unsigned int regs[35]){
	kprintf("#scheduler: \n\tchecking waiting threads, waiting_queue: %i\n", waiting_queue->count);
	if(waiting_queue->count == 0){
		//kprintf("no waiting thread!\n");
		return;
	}
	

	struct list_elem *_temp = waiting_queue->curr;
	for(unsigned int i = 0; i < waiting_queue->count; i++){
		if(_temp->sleep_time == 0){
			kprintf("\tfound waiting thread: thread[%i]\n", _temp->context->id);
			wake_thread(buffer_pull(&uart_input_buffer), _temp, regs);
			return;
		}
		_temp = _temp->next;
	}
	kprintf("no thread is waiting for char!\n");
	return;
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

unsigned int fill_tcb(unsigned char* data, unsigned int count, void (unterprogramm)(unsigned char*)){

	free_tcb->pc = (unsigned int) unterprogramm;
	free_tcb->cpsr = USER_MODE;
	
	free_tcb->sp = USER_STACK_BASE + (free_tcb->id * USER_STACK_SIZE);
	decrease_sp(&free_tcb->sp, (count * sizeof(unsigned char*)));
	kmemcpy((void*)free_tcb->sp, data, count * sizeof(unsigned char*));
	
	free_tcb->registers[0] = free_tcb->sp;
	
	//DEBUG
	free_tcb->data = *data;
	
	free_tcb->in_use = 1;
	return free_tcb->id;
}


void push_tcb_to_ready_queue(unsigned int thread_id, unsigned int irq_regs[]){	
	//no threads active
	if(ready_queue->count == 0){ 							
		ready_queue->curr = &threads[thread_id];
		ready_queue->last = &threads[thread_id];
		ready_queue->curr->next = ready_queue->curr;
		ready_queue->curr->prev = ready_queue->curr;
		load_context(irq_regs, ready_queue->curr->context);
		
	
	//exactly 1 thread active
	} else if(ready_queue->count == 1){ 	
		ready_queue->curr->next = &threads[thread_id];
		ready_queue->curr->prev = &threads[thread_id];
		threads[thread_id].next = ready_queue->curr;
		threads[thread_id].prev = ready_queue->curr;
		ready_queue->last = &threads[thread_id];
	
	//2 or more threads active
	} else{
		if(threads[thread_id].sleep_time == 0){
			ready_queue->curr->next->prev = &threads[thread_id];
			threads[thread_id].next = ready_queue->curr->next;
			ready_queue->curr->next = &threads[thread_id];
			threads[thread_id].prev = ready_queue->curr;
		}else{
			ready_queue->last->next = &threads[thread_id];
			threads[thread_id].prev = ready_queue->last;
			threads[thread_id].next = ready_queue->curr;
			ready_queue->curr->prev = &threads[thread_id];
			ready_queue->last = &threads[thread_id];
		}
	}
	
	ready_queue->count ++;
	threads[thread_id].sleep_time = 0;
}


void create_thread(unsigned char* data, unsigned int count, void (unterprogramm)(unsigned char*), unsigned int irq_regs[]){

	kprintf("create_thread: \n\tgoing to create_thread with %c\n", *data);
	
	if(!find_free_tcb()){
		kprintf("cant create thread! already %i threads running..\n", THREAD_COUNT);
		return;}
		
	//DEBUG
	//print_ready_queue();
	
	unsigned int thread_id = fill_tcb(data, count, unterprogramm);
	push_tcb_to_ready_queue(thread_id, irq_regs);
	used_tcbs ++;
	find_free_tcb();
	
	//DEBUG
	//kprintf("waiting_queue: %i \nready_queue: %i \nready->curr: thread[%i]\n", waiting_queue->count, ready_queue->count, ready_queue->curr->context->id);	
	print_ready_queue();
	print_waiting_queue();
}

unsigned int ready_threads_in_waiting(){
	unsigned int count = 0;
	
	struct list_elem *_temp = waiting_queue->curr;
	for(unsigned int i = 0; i < waiting_queue->count; i++){
		if(_temp->sleep_time == 1){
			count++;
		}
		_temp = _temp->next;
	}
	kprintf("in waiting_queue are %i ready threads\n", count);
	kprintf("\n\n*****************************************************");
	print_waiting_queue();
	return count;
}

void kill_thread(unsigned int regs[]){
	//killing the only thread
	if(ready_queue->count == 1){
		ready_queue->curr->context->in_use = 0;
		ready_queue->curr = 0x0;
		
		if(ready_threads_in_waiting()){
			used_tcbs--;
			ready_queue->count--;
			scheduler(regs);
		}else{
			kprintf("going to load idle_thread\n");
			regs[LR] = (unsigned int) &idle_thread;	
			used_tcbs--;
			ready_queue->count--;
		}
		
		//DEBUG
		//print_ready_queue();
		
	}else if(ready_queue->count > 1){
		ready_queue->curr->context->in_use = 0;
		ready_queue->curr->next->prev = ready_queue->curr->prev;
		ready_queue->curr->prev->next = ready_queue->curr->next;
		ready_queue->curr = ready_queue->curr->next;
		load_context(regs, ready_queue->curr->context);
		
		used_tcbs--;
		ready_queue->count--;
	}
	
	
	
	//DEBUG
	//print_ready_queue();
	
	return;
}

void wait_thread(unsigned int sleep_time, unsigned int regs[]){

	kprintf("#wait_thread: \n\tthread[%i] will wait for %i\n", ready_queue->curr->context->id, sleep_time);
	
	//set sleep_time and update tcb
	ready_queue->curr->sleep_time = sleep_time;
	save_context(regs);
	
	struct list_elem *temp_curr = ready_queue->curr;
	//ready_queue neu sortieren
	if(ready_queue->count == 1){
		ready_queue->curr = 0x0;
		regs[LR] = (unsigned int) &idle_thread;
		
	} else if(ready_queue->count > 1){
		ready_queue->curr->next->prev = ready_queue->curr->prev;
		ready_queue->curr->prev->next = ready_queue->curr->next;
		ready_queue->curr = ready_queue->curr->next;
		load_context(regs, ready_queue->curr->context);
	}
	
	
	if(waiting_queue->count == 0){
		//verschiebe ready_queue->curr zu waiting_queue->curr
		waiting_queue->curr = temp_curr;
		waiting_queue->last = temp_curr;
		waiting_queue->curr->next = temp_curr;
		waiting_queue->curr->prev = temp_curr;
		
	} else if(waiting_queue->count == 1){ 	
		waiting_queue->curr->next = temp_curr;
		waiting_queue->curr->prev = temp_curr;
		temp_curr->next = waiting_queue->curr;
		temp_curr->prev = waiting_queue->curr;
		waiting_queue->last = temp_curr;
		
	}else{
		waiting_queue->last->next = temp_curr;
		temp_curr->prev = waiting_queue->last;
		temp_curr->next = waiting_queue->curr;
		waiting_queue->curr->prev = temp_curr;
		waiting_queue->last = temp_curr;
	}
	
	ready_queue->count--;
	waiting_queue->count++;

	//kprintf("waiting_queue: %i \nready_queue: %i \nready->curr: thread[%i]\n", waiting_queue->count, ready_queue->count, ready_queue->curr->context->id);
	print_ready_queue();
	print_waiting_queue();
	return;
}


void wake_thread(unsigned char _send_char, struct list_elem* _waiting_thread, unsigned int regs[35]){

	kprintf("#wake_thread: \n\tim going to wake thread[%i] with char: %c\n", _waiting_thread->context->id, _send_char);
	//kprintf("\twaiting thread[%i] stackpointer is at %p\n", _waiting_thread->context->id, _waiting_thread->context->sp);
	
	//waiting thread bekommt send_char in $r0
	if(_send_char != '#'){
		_waiting_thread->context->registers[0] = (unsigned int) _send_char;
	}
	
	//print_ready_queue();
	
	//_waiting_thread aus waiting_queue löschen
	if(waiting_queue->count == 1){
		waiting_queue->curr = 0x0;
	} else if(waiting_queue->count > 1){
		_waiting_thread->next->prev = _waiting_thread->prev;
		_waiting_thread->prev->next = _waiting_thread->next;
		if(_waiting_thread == waiting_queue->curr){
			waiting_queue->curr = _waiting_thread->next;
		}
	}
	waiting_queue->count--;
	
	//_waiting_thread in ready_queue einordnen
	unsigned int _thread_id = _waiting_thread->context->id;
	push_tcb_to_ready_queue(_thread_id, regs);
	
	
	//kprintf("waiting_queue: %i \nready_queue: %i \nready->curr: thread[%i]\n", waiting_queue->count, ready_queue->count, ready_queue->curr->context->id);
	print_ready_queue();
	print_waiting_queue();
}

/*
kprintf("\n********************* AFTER *************************\n");
	kprintf("waiting_queue: %i \nready_queue: %i \nready->curr: thread[%i]\n", waiting_queue->count, ready_queue->count, ready_queue->curr->context->id);
	print_ready_queue();
	kprintf("*****************************************************\n\n");
*/
