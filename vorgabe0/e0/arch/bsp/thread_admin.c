#include <lib/unterprogramm.h>
#include <lib/kmemcpy.h>
#include <kernel/kprintf.h>
#include <kernel/idle_thread.h>

#define LR		21
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
	kprintf("\n");
}

void change_context(unsigned int regs[]){
	save_context(regs);
	load_context(regs, ready_queue->curr->next->context);
	//change pointer positions
	ready_queue->curr = ready_queue->curr->next;
	ready_queue->last = ready_queue->curr->prev;
}

void scheduler(unsigned int regs[]){
	//no thread active
	if(ready_queue->curr == 0x0) return;
	
	//exactly 1 thread active
	else if(ready_queue->curr == ready_queue->curr->next) return;
	
	//more than 1 thread active
	else if(ready_queue->curr != ready_queue->curr->next){
		change_context(regs);
		kprintf("\n");
	}
	return;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/ THREAD ADMINISTRATION /_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

void print_list_elem(int i){
	kprintf("\t[%i]: %c\n", i, (char) tcbs[i].registers[0]);
}

void print_ready_queue(){
	kprintf("queue changed to %i active threads:\n", used_tcbs);
	for(int i = 0; i < used_tcbs; i++){
		print_list_elem(i);
	}
}


int find_free_tcb(){
	for(int i = 0; i < 32; i++){
		if(tcbs[i].in_use == 0){
			free_tcb = &tcbs[i];
			return 1;
		}
	}
	free_tcb = 0x0;
	return 0;
}

unsigned int fill_tcb(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char)){
	free_tcb->pc = (unsigned int) unterprogramm;
	kmemcpy(&(free_tcb->registers[0]), &data, count * sizeof(unsigned char*));
	free_tcb->in_use = 1;
	return free_tcb->id;
}

void push_tcb_to_ready_queue(unsigned int thread_id, unsigned int irq_regs[]){	
	//no threads active
	if(ready_queue->curr == 0x0){ 	
		kprintf("this is the only thread.\n");							
		ready_queue->curr = &threads[thread_id];
		ready_queue->last = &threads[thread_id];
		ready_queue->curr->next = ready_queue->curr;
		ready_queue->curr->prev = ready_queue->curr;
		load_context(irq_regs, ready_queue->curr->context);
		print_ready_queue();
	
	//exactly 1 thread active
	} else if(ready_queue->curr == ready_queue->last){ 	
		ready_queue->curr->next = &threads[thread_id];
		ready_queue->curr->prev = &threads[thread_id];
		threads[thread_id].next = ready_queue->curr;
		threads[thread_id].prev = ready_queue->curr;
	
	//more than 2 threads active
	} else{
		ready_queue->last->next = &threads[thread_id];
		threads[thread_id].prev = ready_queue->last;
		ready_queue->last = &threads[thread_id];
	}
}

void create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char), unsigned int irq_regs[]){
	if(!find_free_tcb()){
		kprintf("cant create thread! already 32 threads running..\n");
		return;}
		
	//kprintf("create thread %i with char: %c\n", free_tcb->id, *data);
	unsigned int thread_id = fill_tcb(data, count, unterprogramm);
	used_tcbs ++;
	push_tcb_to_ready_queue(thread_id, irq_regs);
	//kprintf("3 leave thread admin\n");
}

void kill_thread(unsigned int regs[]){
	if(used_tcbs == 2){
		//kprintf("exactly two threads remaining.\n");
		ready_queue->curr->next->prev = 0x0;
		ready_queue->curr->prev->next = 0x0;		
	} else if(used_tcbs > 2){
			//kprintf("more than two threads remaining.\n");
		ready_queue->curr->next->prev = ready_queue->curr->prev;
		ready_queue->curr->prev->next = ready_queue->curr->next;
	}
	ready_queue->curr->context->in_use = 0;
	used_tcbs--;
	ready_queue->curr->prev = 0x0;
	struct list_elem *temp = ready_queue->curr->next;
	ready_queue->curr->next = 0x0;
	ready_queue->curr = temp;
	if(!ready_queue->curr){
		//kprintf("last thread killed.\n");
	}	
	if(ready_queue->curr){
		//kprintf("at least one thread remaining.\n");
		load_context(regs, ready_queue->curr->context);
		return;
	}
	kprintf("return to idle thread \n");
	regs[LR] = (unsigned int) &idle_thread;		
	return;
}













