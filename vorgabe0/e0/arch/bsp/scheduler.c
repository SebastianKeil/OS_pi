#include <arch/bsp/scheduler.h>
#include <arch/bsp/thread_admin.h>

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

void change_context(struct *ready_queue, unsigned int regs[35]){
	
	//save old context
	ready_queue->curr->context->pc = regs[PC];
	ready_queue->curr->context->sp = regs[SP];
	ready_queue->curr->context->cpsr = regs[CPSR];
	kmemcopy(&(ready_queue->curr->context->registers), &regs[22], 13*sizeof(unsigned int));

	//load new context
	regs[PC] = ready_queue->curr->next->context->pc;
	regs[SP] = ready_queue->curr->next->context->sp;
	regs[CPSR] = ready_queue->curr->next->context->cpsr;
	kmemcopy(&regs[22], &(ready_queue->curr->next->context->registers), 13*sizeof(unsigned int));

	//change pointer positions
	ready_queue->curr = ready_queue->curr->next;
	ready_queue->last = ready_queue->curr->prev;
}

void scheduler(struct *ready_queue, unsigned int regs[35]);{
	if(ready_queue->curr && ready_queue->curr->next){
		change_context(&ready_queue, regs[35]); //ready_queue hier so übergeben???
	}
	return;
}
