#ifndef _THREAD_ADMIN_H
#define _THREAD_ADMIN_H

//////////////////////_/
/////////  DATA //////////_/
//////////////////////_/
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



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/ DEBUG /_/_/_/_/_/_/_/_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
void print_list_elem(unsigned int _j);
void print_ready_queue(void);


//////////////////////_/
////////// SETUP /////////_/
//////////////////////_/
void init_thread_admin(void);
void init_ready_queue(void);
void init_waiting_queue(void);
void init_all_tcbs(void);
void init_thread_slots();


//////////////////////_/
///////// SCHEDULER ////////_/
//////////////////////_/
void safe_context(unsigned int regs[]);
void load_context(unsigned int regs[], struct tcb* context);
void change_context(unsigned int regs[35]);
void scheduler(unsigned int regs[35]);
void check_for_waiting_threads(unsigned int regs[35]);

//////////////////////_/
////// THREAD ADMINISTRATION /////_/
//////////////////////_/
int find_free_tcb(void);
void decrease_sp(unsigned int* _sp, unsigned int size);
unsigned int fill_tcb(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*));
void push_tcb_to_ready_queue(unsigned int thread_id, unsigned int regs[]);
void create_thread(unsigned char* data, unsigned int count, void (*unterprogramm)(unsigned char*), unsigned int regs[]);
void kill_thread(unsigned int regs[]);
void wait_thread(unsigned int sleep_time, unsigned int regs[]);
void wake_thread(unsigned char _send_char, struct list_elem* _temp, unsigned int regs[35]);

#endif
