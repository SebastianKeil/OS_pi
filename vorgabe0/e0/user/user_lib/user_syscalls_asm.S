

.globl syscall_get_char_asm
syscall_get_char_asm:
    svc #43
    bx lr
