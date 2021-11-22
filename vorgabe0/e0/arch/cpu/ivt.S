.global _ivt
.balign 64
_ivt:
	b reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq
_undefined_instruction: .word undefined_instruction
_software_interrupt: 	.word software_interrupt
_abort: 				.word abort
_not_used: 				.word not_used
_irq: 					.word irq
_fiq: 					.word fiq


/*Code übernommen aus https://isis.tu-berlin.de/pluginfile.php/2119612/mod_resource/content/2/02-arm-debugging.pdf von S.Bauroth*/

reset:

