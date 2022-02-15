

.globl syscall_get_char_asm
syscall_get_char_asm:
	push {r2, lr}
    svc #43
    pop {r2, pc}
    
  
.globl syscall_sleep_thread_asm
syscall_sleep_thread_asm:
	push {r2, lr}
	svc #45
	pop {r2, pc} 
	
.globl syscall_create_thread_asm
syscall_create_thread_asm:
	push {r2, lr}
	svc #44
	pop {r2, pc} 
	
.globl syscall_ufork_asm
syscall_ufork_asm:
	push {r2, lr}
	svc #420
	pop {r2, pc} 
