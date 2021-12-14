#ifndef _THREAD_ADMIN_H
#define _THREAD_ADMIN_H

void init_all_tcbs(void);
void create_thread(unsigned char receive_buffer, void (*unterprogramm)());

#endif
