.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom PC notwendig

ldr		r0, =0x1
ldr		r3, =0xA
ldr		r4, =0xABC
ldr 	r5, =0xFEF
ldr		r6, =0xABC
stmdb 	sp!, {r0-r15} 

//ldr		r0, cpsr
//mrs		r1, spsr_und
stmdb	sp!, {r0, r1} //store cpsr and spsr_und on stack
mov		r0, sp //give sp to c function

bl 		software_interrupt //evtl mov ldr statt bl
	
//ldmib	sp!, {r0, r1} //remove unnecessary items on stack
//stmdb 	sp!, {r14} //load LR to stack 
//ldmib	sp, {r0-r12,r15}^