.global irq_asm

irq_asm:

sub 	r14, r14, #4  //Offset abziehen für nahtlose Fortführung
	
//getrenntes storen sonst writebackkomplikationen, falls sp nicht niedrigstes reg in target list
stmfd	sp!, {r13-r15} //Print I Reglist, Index 19-34
stmfd 	sp!, {r0-r12}

mrs		r12, cpsr //Print II aktuell, Index 17-18
mrs		r11, spsr
mrs		r10, lr_usr //Print III Modusspez., Index 0-16
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
mrs		r3, spsr //für ein einheitliches in anderen Modi ausprinten wiederholt
mrs		r2, lr_und
mrs		r1, sp_und
mrs		r0, spsr_und

stmfd	sp!, {r0-r5}

mov		r0, sp //give sp to c function

bl 		irq //evtl mov ldr statt bl	

ldmfd	sp!, {r0 - r5} //remove unnecessary items from stack
ldmfd	sp!, {r0- r12} //remove unnecessary items from stack
ldmfd	sp!, {r0- r12} //load necesessary registers before mode change
mov		r0, sp //give sp to c function
ldr 	sp, =25000 //reset Stack Pointer
bl		irq_test
movs	pc, lr






