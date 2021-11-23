.global _ivt
.balign 64
_ivt:
	b reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _abort
	ldr pc, _irq
	ldr pc, _fiq
_undefined_instruction: .word undefined_instruction_asm
_software_interrupt: 	.word software_interrupt_asm
_abort: 				.word abort_asm
_irq: 					.word irq_asm
_fiq: 					.word fiq_asm


/*Code übernommen aus https://isis.tu-berlin.de/pluginfile.php/2119612/mod_resource/content/2/02-arm-debugging.pdf von S.Bauroth*/

reset:
//inactiv irq and fiq, switch to svc mode and jump to _start
