#include <lib/unterprogramm.h>
#include <lib/kmemcpy.h>

struct tcb{
	//thread's context
	unsigned int id;
	unsigned int in_use;
	unsigned int* pc;
	unsigned int* sp;
	unsigned int cpsr;
	unsigned int registers[13];
};

struct tcb tcbs[33]; //32 threads supported
unsigned int used_tcbs;
struct tcb * free_tcb; //first free tcb slot


struct list_elem{
	struct list_elem * next;
	struct list_elem * prev;
	struct tcb * context;
};

struct list{
	struct list_elem * curr;
	struct list_elem * last;
	//struct list_elem * elements[32];
};

struct list ready_queue;

void init_ready_queue(){
	ready_queue.curr = (struct list_elem*) 0x0;
	ready_queue.last = (struct list_elem*) 0x0;
}

void init_all_tcbs(){
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
	/*
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
	
	//initialize their stackpointers and ids
	for(unsigned int i = 0; i < 32; i++){
		tcbs[i].sp = (unsigned int*) 0x27000 + (i * 0x1000);
		tcbs[i].id = i;
		tcbs[i].in_use = 0;
	}
	
	//set next free tcb to tcb0
	free_tcb = &tcbs[0];
	used_tcbs = 0;
}

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
