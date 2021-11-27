.global irq_asm

irq_asm:

stmdb	sp!, {r13-r15} //Print I Reglist, Index 19-34
stmdb 	sp!, {r0-r12}

//stmda 	sp!, {r0-r12}
//sub r14, r14, #8  //Offset abziehen für nahtlose Fortführung
//TODO evtl mov ldr statt bl


mov		r0, sp //give sp to c function
bl 		irq

//stmdb 	sp!, {r14} //load LR to stack 
//ldmib	sp, {r0-r12,r15}^

//TODO irq Leitung zurücksetzen
mov 	sp, r14
ldmib	sp, {r0-r12,r15}^





