.global fiq_asm

fiq_asm:

stmda 	sp!, {r0-r12}
//TODO fiq ausschalten?
sub r14, r14, #8  //Offset abziehen für nahtlose Fortführung
//TODO evtl mov ldr statt bl
bl 		fiq

//TODO fiq Leitung zurücksetzen?
mov 	sp, r14
ldmib	sp!, {r0-r12,r15}^
