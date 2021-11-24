.global data_abort_asm

data_abort_asm:

//TODO sofort unterscheiden zwischen prefetch und dataabort
//Dementsprechend springen

//stmda 	sp!, {r0-r12}

//ldr 	r0, 

//bl 		data_abort
//sub 	r14, r14, #8
//bl		prefetch_abort
//sub 	r14, r14, #4

//ldmda	sp!, {r0-r12}
