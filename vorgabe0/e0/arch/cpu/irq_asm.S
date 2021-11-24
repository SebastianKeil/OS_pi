.global irq_asm

irq_asm:

stmda 	sp!, {r0-r12}
//TODO irq ausschalten
sub r14, r14, #8  //Offset abziehen für nahtlose Fortführung
//TODO evtl mov ldr statt bl
bl 		irq

//TODO irq Leitung zurücksetzen
mov 	sp, r14
ldmib	sp!, {r0-r12,r15}^





