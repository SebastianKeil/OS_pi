.global abort

abort:

//TODO sofort unterscheiden zwischen prefetch und dataabort
//Dementsprechend springen

//stmda 	sp!, {r0-r12}

//bl 		data_abort
//bl		prefetch_abort

//ldmda	sp!, {r0-r12}
