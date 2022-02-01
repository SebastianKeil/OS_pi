

.globl syscall_get_char_asm
syscall_get_char_asm:
	push {r2, lr}
    svc #43
    pop {r2, pc}
    
    
    
