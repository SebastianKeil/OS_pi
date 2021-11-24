.global undefined_instruction_asm

undefined_instruction_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom PC notwendig
stmda 	sp!, {r0-r12}

//evtl move ldr statt bl
bl 		undefined_instruction

mov 	sp, r14
ldmib	sp!, {r0-r12,r15}^