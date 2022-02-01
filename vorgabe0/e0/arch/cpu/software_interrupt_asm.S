.global software_interrupt_asm

software_interrupt_asm:
//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom LR/PC notwendig
sub 	r14, r14, #0  	//Offset abziehen für nahtlose Fortführung
						//					Index auf Stack:
stmfd 	sp!, {r0-r12}	//Print I Reglist 	22-34
stmfd	sp!, {lr}		//					21
stmfd	sp!, {r13,r15}  //	 				pc 20, sp 19	

mrs		r12, cpsr 		//Print II aktuell, 17-18
mrs		r11, spsr
mrs		r10, lr_usr 	//Print III Modus,	0-16
mrs		r9, sp_usr
mov		r8, lr
mov		r7, sp
mrs		r6, spsr 
mrs		r5, lr_abt
mrs		r4, sp_abt
mrs		r3, spsr_abt
mrs		r2, lr_fiq
mrs		r1, sp_fiq
mrs		r0, spsr_fiq
stmfd	sp!, {r0-r12}

mrs		r5, lr_irq
mrs		r4, sp_irq
mrs		r3, spsr_irq
mrs		r2, lr_und
mrs		r1, sp_und
mrs		r0, spsr_und
stmfd	sp!, {r0-r5}

mov		r0, sp //give sp to c function

bl 		software_interrupt


ldmfd	sp!, {r0 - r7}
ldmfd	sp!, {r0 - r6}  //remove unnecessary items from stack

ldmfd	sp!, {r1}		//load sp_usr
msr 	sp_usr, r1 		//set sp_usr to sp of coming thread

ldmfd	sp!, {r1}		//remove

ldmfd	sp!, {r0}		//load new spsr
msr		SPSR_cxsf, r0

ldmfd	sp!, {r0 - r2} 	// remove unnecessary items from stack

ldmfd	sp!, {lr}		//load necesessary registers before 
ldmfd	sp!, {r0- r12} 
movs	pc, lr
