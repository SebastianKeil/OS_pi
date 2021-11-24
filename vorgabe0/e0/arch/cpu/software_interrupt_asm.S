.global software_interrupt_asm

software_interrupt_asm:

//Undef/SVC in Decode erkannt also auslösende Instruktion + 4, 
//aber Instruktion soll übersprungen werden => keine Manipulation vom PC notwendig
stmda 	sp!, {r0-r12} //zeigt sp auf das letzte geschrieben Element oder auf neuen freien Platz? remove line 9?
mov		r0, sp
sub		r0, r0, #4    //point to last written element of stack



//mov		r1, //Hier kommen die Flags rein ____ ____ data, prefetch


//evtl mov ldr statt bl
bl 		software_interrupt

mov 	sp, r14
ldmib	sp, {r0-r12,r15}^