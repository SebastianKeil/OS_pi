.global software_interrupt_asm

software_interrupt_asm:

stmda 	sp!, {r0-r12}

//evtl mov ldr statt bl
bl 		software_interrupt

mov 	sp, r14
ldmib	sp!, {r0-r12,r15}^