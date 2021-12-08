



struct tcb{
	//Kontext eines Threads
	unsigned int pc;
	unsigned int sp;
	unsigned int cpsr;
	unsigned int registers[13];
}

struct tcb * running;
struct tcb tcbs[32];

struct list_elem{
	struct list_elem * next;
	struct list_elem * prev;
	struct tcb * context;
}

struct list{
	struct list_elem * first;
	struct list_elem * free; //erste freie
	//struct list_elem * elements[32];
}
void create_thread(unsigned char receive_buffer){
	//neues struct tcb erstellen mit gesamtem kontext für diesen thread
	//diesen tcb in die ready-list pushen
}
