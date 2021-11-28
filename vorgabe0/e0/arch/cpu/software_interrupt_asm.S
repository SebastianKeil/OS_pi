.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom LR/PC notwendig
//getrenntes storen sonst writebackkomplikationen, falls sp nicht niedrigstes reg in target list
//add		r14, r14, #4
stmfd	sp!, {r13-r15} //Print I Reglist, Index 19-34
stmfd 	sp!, {r0-r12}

mrs		r12, cpsr //Print II aktuell, Index 17-18
mrs		r11, spsr

mrs		r10, lr_usr //Print III Modusspez., Index 0-16
mrs		r9, sp_usr
mov		r8, r14
mov		r7, r13
mrs		r6, spsr //für ein einheitliches in anderen Modi ausprinten wiederholt
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

/*
ldmfd	sp!, {r0 - r5} //remove unnecessary items from stack
ldmfd	sp!, {r0- r12} //remove unnecessary items from stack
mov		r0, sp //give sp to c function
bl 		irq_test
ldmfd	sp!, {r0- r12} //load necesessary registers before mode change
mov		r0, sp //give sp to c function
bl 		irq_test

ldr		lr, =8194 //harter cheat :)
movs	pc, lr
*/