

struct tcb{
	//thread's context
	unsigned int id;
	unsigned int pc;
	unsigned int* sp;
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
	
	tcb0.sp =  0x27000;
	tcb1.sp =  0x28000;
	tcb2.sp =  0x29000;
	tcb3.sp =  0x30000;
	tcb4.sp =  0x31000;
	tcb5.sp =  0x32000;
	tcb6.sp =  0x33000;
	tcb7.sp =  0x34000;
	tcb8.sp =  0x35000;
	tcb9.sp =  0x36000;
	tcb10.sp = 0x37000;
	tcb11.sp = 0x38000;
	tcb12.sp = 0x39000;
	tcb13.sp = 0x40000;
	tcb14.sp = 0x41000;
	tcb15.sp = 0x42000;
	tcb16.sp = 0x43000;
	tcb17.sp = 0x44000;
	tcb18.sp = 0x45000;
	tcb19.sp = 0x46000;
	tcb20.sp = 0x47000;
	tcb21.sp = 0x48000;
	tcb22.sp = 0x49000;
	tcb23.sp = 0x50000;
	tcb24.sp = 0x51000;
	tcb25.sp = 0x52000;
	tcb26.sp = 0x53000;
	tcb27.sp = 0x54000;
	tcb28.sp = 0x55000;
	tcb29.sp = 0x56000;
	tcb30.sp = 0x57000;
	tcb31.sp = 0x58000;
}

void init_tcb(unsigned char data){
	//TODO: übergebenes zeichen auf den stack
}

void create_thread(unsigned char receive_buffer, ){
	init_tcb();
	push_tcb_to_queue();
	
}
