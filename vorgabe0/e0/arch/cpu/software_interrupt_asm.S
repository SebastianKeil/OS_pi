.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom PC notwendig
stmda 	sp!, {r0-r12}

//evtl mov ldr statt bl
bl 		software_interrupt

mov 	sp, r14
ldmib	sp, {r0-r12,r15}^