.global irq_asm

irq_asm:

sub 	r14, r14, #8  	//Offset abziehen für nahtlose Fortführung
						//					Index auf Stack:
stmfd 	sp!, {r0-r12}	//Print I Reglist 	22-34
stmfd	sp!, {lr}		//					21
stmfd	sp!, {r13,r15}  //	 				pc 20, sp 19	

mrs		r12, cpsr 		//Print II aktuell, 17-18
mrs		r11, spsr
mrs		r10, lr_usr 	//Print III Modus,	0-16
mrs		r9, sp_usr
mrs		r8, lr_svc
mrs		r7, sp_svc
mrs		r6, spsr_svc 
mrs		r5, lr_abt
mrs		r4, sp_abt
mrs		r3, spsr_abt
mrs		r2, lr_fiq
mrs		r1, sp_fiq
mrs		r0, spsr_fiq

stmfd	sp!, {r0-r12}

mov		r5, r14
mov		r4, r13
mrs		r3, spsr
mrs		r2, lr_und
mrs		r1, sp_und
mrs		r0, spsr_und

stmfd	sp!, {r0-r5}

mov		r0, sp //give sp to c function

bl 		irq //evtl mov ldr statt bl	

ldmfd	sp!, {r0 - r7} 	//remove unnecessary items from stack
ldmfd	sp!, {r0- r12} 
ldmfd	sp!, {lr}		//load necesessary registers before 
ldmfd	sp!, {r0- r12} 
movs	pc, lr