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
_unused:
_irq: 					.word irq_asm
_fiq: 					.word fiq_asm



/*Code übernommen aus https://isis.tu-berlin.de/pluginfile.php/2119612/mod_resource/content/2/02-arm-debugging.pdf von S.Bauroth*/

reset:
//inactiv irq and fiq, switch to svc mode and jump to _start


/*Reset svc 0x00
Undefined Instruction und 0x04
Software Interrupt (SWI) svc 0x08
Prefetch Abort abt 0x0C
Data Abort abt 0x10
IRQ irq 0x18
FIQ fiq 0x1C*/
