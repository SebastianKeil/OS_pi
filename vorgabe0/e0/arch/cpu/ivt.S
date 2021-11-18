.globl _ivt
.balign 64
_ivt:
b _reset_handler
ldr pc, _undef_handler
//TODO...
