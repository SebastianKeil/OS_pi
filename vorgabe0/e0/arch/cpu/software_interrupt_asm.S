.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom LR/PC notwendig
//getrenntes storen sonst writebackkomplikationen, falls sp nicht niedrigstes reg in target list

stmdb	sp!, {r13-r15} //Print I Reglist, Index 19-34
stmdb 	sp!, {r0-r12}

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

stmdb	sp!, {r0-r12}

mrs		r5, lr_irq
mrs		r4, sp_irq
mrs		r3, spsr_irq
mrs		r3, lr_und
mrs		r2, sp_und
mrs		r0, spsr_und

stmdb	sp!, {r0-r5}

mov		r0, sp //give sp to c function

bl 		software_interrupt //evtl mov ldr statt bl	
//ldmib	sp!, {r0, r1} //remove unnecessary items on stack
//stmdb 	sp!, {r14} //load LR to stack 
//ldmib	sp, {r0-r12,r15}^


/*
r8_usr 	sp_irq
r9_usr 	lr_svc
r10_usr sp_svc
r11_usr lr_abt
r12_usr sp_abt
sp_usr 	lr_und
lr_usr 	sp_und
r8_fiq 	spsr_fiq
r9_fiq 	spsr_irq
r10_fiq spsr_svc
r11_fiq spsr_abt
r12_fiq spsr_und
sp_fiq 	lr_irq
lr_fiq
*/

