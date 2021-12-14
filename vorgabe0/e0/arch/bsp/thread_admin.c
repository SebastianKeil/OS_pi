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


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/ SETUP /_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

void init_ready_queue(){
	ready_queue->curr = (struct list_elem*) 0x0;
	ready_queue->last = (struct list_elem*) 0x0;
}

void init_all_tcbs(){
	for(unsigned int i = 0; i < 31; i++){
		tcbs[i].sp = (unsigned int*) 0x27000 + (i * 0x1000);
		tcbs[i].id = i;
		tcbs[i].in_use = 0;
	}
	
	free_tcb = &tcbs[0];
	used_tcbs = 0;
}


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/ SCHEDULER /_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

void change_context(unsigned int regs[]){
	
	//save old context
	(ready_queue->curr->context->pc) = regs[PC];
	(ready_queue->curr->context->sp) = regs[SP];
	(ready_queue->curr->context->cpsr) = regs[CPSR];
	kmemcpy(&(ready_queue->curr->context->registers), &regs[22], 13*sizeof(unsigned int));
	

	//load new context
	regs[PC] = (ready_queue->curr->next->context->pc);
	regs[SP] = (ready_queue->curr->next->context->sp);
	regs[CPSR] = (ready_queue->curr->next->context->cpsr);
	kmemcpy(&regs[22], &(ready_queue->curr->next->context->registers), 13*sizeof(unsigned int));

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

void find_free_tcb(){
	for(int i = 0; i < 32; i++){
		if(tcbs[i].in_use == 0){
			free_tcb = &tcbs[i];
		}
	}
}

unsigned int fill_tcb(unsigned char* data, unsigned int count, void (*unterprogramm)()){
	free_tcb->pc = (unsigned int*) unterprogramm;
	//push_to_thread_stack(data, count);
	free_tcb->in_use = 1;
	return free_tcb->id;
}

void push_tcb_to_ready_queue(unsigned int thread_id){
	
}

void create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)()){
	unsigned int thread_id = fill_tcb(data, count, unterprogramm);
	find_free_tcb();
	used_tcbs ++;
	//creat
	
	push_tcb_to_ready_queue(thread_id);
	

}



























/*
	//declaration of 32 tcbs
	struct tcb tcb0;
	struct tcb tcb1;
	struct tcb tcb2;
	struct tcb tcb3;
	struct tcb tcb4;
	struct tcb tcb5;
	struct tcb tcb6;
	struct tcb tcb7;
	struct tcb tcb8;
	struct tcb tcb9;
	struct tcb tcb10;
	struct tcb tcb11;
	struct tcb tcb12;
	struct tcb tcb13;
	struct tcb tcb14;
	struct tcb tcb15;
	struct tcb tcb16;
	struct tcb tcb17;
	struct tcb tcb18;
	struct tcb tcb19;
	struct tcb tcb20;
	struct tcb tcb21;
	struct tcb tcb22;
	struct tcb tcb23;
	struct tcb tcb24;
	struct tcb tcb25;
	struct tcb tcb26;
	struct tcb tcb27;
	struct tcb tcb28;
	struct tcb tcb29;
	struct tcb tcb30;
	struct tcb tcb31;
	
	kmemcpy(&tcbs[0], &tcb0, sizeof(struct tcb));
	tcbs[0] =  tcb0;
	tcbs[1] =  tcb1;
	tcbs[2] =  tcb2;
	tcbs[3] =  tcb3;
	tcbs[4] =  tcb4;
	tcbs[5] =  tcb5;
	tcbs[6] =  tcb6;
	tcbs[7] =  tcb7;
	tcbs[8] =  tcb8;
	tcbs[9] =  tcb9;
	tcbs[10] = tcb10;
	tcbs[11] = tcb11;
	tcbs[12] = tcb12;
	tcbs[13] = tcb13;
	tcbs[14] = tcb14;
	tcbs[15] = tcb15;
	tcbs[16] = tcb16;
	tcbs[17] = tcb17;
	tcbs[18] = tcb18;
	tcbs[19] = tcb19;
	tcbs[20] = tcb20;
	tcbs[21] = tcb21;
	tcbs[22] = tcb22;
	tcbs[23] = tcb23;
	tcbs[24] = tcb24;
	tcbs[25] = tcb25;
	tcbs[26] = tcb26;
	tcbs[27] = tcb27;
	tcbs[28] = tcb28;
	tcbs[29] = tcb29;
	tcbs[30] = tcb30;
	tcbs[31] = tcb31;
	
	tcbs[0] =  tcb0;
	tcbs[1] =  tcb1;
	tcbs[2] =  tcb2;
	tcbs[3] =  tcb3;
	tcbs[4] =  tcb4;
	tcbs[5] =  tcb5;
	tcbs[6] =  tcb6;
	tcbs[7] =  tcb7;
	tcbs[8] =  tcb8;
	tcbs[9] =  tcb9;
	tcbs[10] = tcb10;
	tcbs[11] = tcb11;
	tcbs[12] = tcb12;
	tcbs[13] = tcb13;
	tcbs[14] = tcb14;
	tcbs[15] = tcb15;
	tcbs[16] = tcb16;
	tcbs[17] = tcb17;
	tcbs[18] = tcb18;
	tcbs[19] = tcb19;
	tcbs[20] = tcb20;
	tcbs[21] = tcb21;
	tcbs[22] = tcb22;
	tcbs[23] = tcb23;
	tcbs[24] = tcb24;
	tcbs[25] = tcb25;
	tcbs[26] = tcb26;
	tcbs[27] = tcb27;
	tcbs[28] = tcb28;
	tcbs[29] = tcb29;
	tcbs[30] = tcb30;
	tcbs[31] = tcb31;
	*/
