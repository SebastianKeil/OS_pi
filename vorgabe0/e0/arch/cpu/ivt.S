.global _ivt
.balign 64
_ivt:
	b reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _unused
	ldr pc, _irq
	ldr pc, _fiq
_undefined_instruction: .word undefined_instruction_asm
_software_interrupt: 	.word software_interrupt_asm
_prefetch_abort:		.word prefetch_abort_asm
_data_abort: 			.word data_abort_asm
_unused:				.word 0xFFFF //platzhalter
_irq: 					.word irq_asm
_fiq: 					.word fiq_asm


/*Code übernommen aus https://isis.tu-berlin.de/pluginfile.php/2119612/mod_resource/content/2/02-arm-debugging.pdf von S.Bauroth*/

reset:
b _start
