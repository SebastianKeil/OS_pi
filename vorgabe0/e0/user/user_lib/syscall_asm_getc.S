.global syscall_asm_getc

syscall_asm_getc:
	push {lr}
	svc #43
	//mov %0,r0
	pop {lr}
