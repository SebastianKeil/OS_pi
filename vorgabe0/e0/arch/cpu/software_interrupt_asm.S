.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom PC notwendig
stmdb 	sp!, {r0-r12} 

mov		r0, cpsr
mrs		r1, spsr_und

stmdb	sp!, {r0, r1} //store cpsr and spsr_und on stack

//evtl mov ldr statt bl
bl 		software_interrupt
	
ldmib	sp!, {r0, r1} //remove unnecessary items on stack
stmdb 	sp!, {r14} //load LR to stack 
ldmib	sp, {r0-r12,r15}^