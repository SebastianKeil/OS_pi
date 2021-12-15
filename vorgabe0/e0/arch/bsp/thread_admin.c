#include <lib/unterprogramm.h>
#include <lib/kmemcpy.h>
#include <kernel/kprintf.h>

#define PC		20
#define SP		19
#define CPSR	18

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

struct tcb{
	//thread's context
	unsigned int id;
	unsigned int in_use;
	unsigned int pc;
	unsigned int sp;
	unsigned int cpsr;
	unsigned int registers[13];
};
struct tcb tcbs[32];
struct tcb *free_tcb; //first free tcb slot
unsigned int used_tcbs;


struct list_elem{
	struct list_elem *next;
	struct list_elem *prev;
	struct tcb *context;
};
struct list{
	struct list_elem *curr;
	struct list_elem *last;
	//struct list_elem * elements[32];
};
struct list *ready_queue;
struct list_elem threads[32];


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/ SETUP /_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

void init_ready_queue(){
	ready_queue->curr = 0x0;
	ready_queue->last = 0x0;
}

void init_all_tcbs(){
	for(unsigned int i = 0; i < 31; i++){
		tcbs[i].sp = 0x27000 + (i * 0x1000);
		tcbs[i].id = i;
		tcbs[i].in_use = 0;
	}
	
	free_tcb = &tcbs[0];
	used_tcbs = 0;
}
void init_thread_slots(){
	for(int i = 0; i < 32; i++){
			threads[i].context = &tcbs[i];
	}
}

void init_thread_admin(){
	kprintf("INIT THREAD ADMIN\n");
	init_ready_queue();
	kprintf("now ready_queue->curr: %p\n", ready_queue->curr);
	init_all_tcbs();
	init_thread_slots();
}

/*
void init_threads_queue(){
	threads[31]->next = threads[0];
	threads[0]->prev = threads[31];
	for(int i = 1; i < 31; i++){
		threads[i]->next = &threads[i + 1];
		threads[i]->prev = &threads[i - 1];
	}	
}
*/

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/ SCHEDULER /_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


void save_context(unsigned int regs[]){
	(ready_queue->curr->context->pc) = regs[PC];
	(ready_queue->curr->context->sp) = regs[SP];
	(ready_queue->curr->context->cpsr) = regs[CPSR];
	kmemcpy(&(ready_queue->curr->context->registers), &regs[22], 13*sizeof(unsigned int));
}

void load_context(unsigned int regs[]){
	regs[PC] = (ready_queue->curr->next->context->pc);
	regs[SP] = (ready_queue->curr->next->context->sp);
	regs[CPSR] = (ready_queue->curr->next->context->cpsr);
	kmemcpy(&regs[22], &(ready_queue->curr->next->context->registers), 13*sizeof(unsigned int));
}

void change_context(unsigned int regs[]){
	save_context(regs);
	load_context(regs);
	//change pointer positions
	ready_queue->curr = ready_queue->curr->next;
	ready_queue->last = ready_queue->curr->prev;
}

void scheduler(unsigned int regs[]){
	if(ready_queue->curr && ready_queue->curr->next){
		change_context(regs);
		kprintf("\n");
	}
	return;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/ THREAD ADMINISTRATION /_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

int find_free_tcb(){
	for(int i = 0; i < 32; i++){
		if(tcbs[i].in_use == 0){
			free_tcb = &tcbs[i];
			return 1;
		}
	}
	return 0;
}

unsigned int fill_tcb(unsigned char* data, void (*unterprogramm)(unsigned char)){
	free_tcb->pc = (unsigned int) unterprogramm;
	free_tcb->registers[0] = (unsigned int) data;
	free_tcb->in_use = 1;
	return free_tcb->id;
}

void push_tcb_to_ready_queue(unsigned int tcb_id, unsigned int regs[]){
	kprintf("1push_tcb_to_queue..\n");
	kprintf("still ready_queue->curr: %p\n", ready_queue->curr);
	kprintf("&threads[tcb_id]: %p\n", &threads[tcb_id]);
	if(ready_queue->curr == 0x0){ //no other threads
		kprintf("2 changing registers\n");
		ready_queue->curr = &threads[tcb_id];
		ready_queue->last = &threads[tcb_id];
		regs[PC] = (ready_queue->curr->context->pc);
		regs[SP] = (ready_queue->curr->context->sp);
		regs[CPSR] = (ready_queue->curr->context->cpsr);
		kmemcpy(&regs[22], &(ready_queue->curr->context->registers), 13*sizeof(unsigned int));
	} else {
		ready_queue->last->next = &threads[tcb_id];
		threads[tcb_id].prev = ready_queue->last;
		ready_queue->last = &threads[tcb_id];
	}
}

void create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char), unsigned int regs[]){
	kprintf("create thread: ready_queue->curr: %p\n", ready_queue->curr);
	
	if(free_tcb == 0x0){
		kprintf("cant create thread! already 32 threads running..\n");
		return;
	}

	kprintf("create thread %i with char: %c\n", free_tcb->id, *data);
	unsigned int tcb_id = fill_tcb(data, unterprogramm);
	used_tcbs ++;
	
	if(!find_free_tcb()){
		kprintf("all tcbs in use!\n");
		free_tcb = (struct tcb*) 0x0;
	}
	
	push_tcb_to_ready_queue(tcb_id, regs);
	kprintf("3 leave thread admin\n");
}

void kill_thread(){
	//sort list, than systimer
}


