.section .init

.global _start

_start:
	/* Erkennen ob Hypervisor Mode aktiv */
	mrs r0, cpsr
	and r0, r0, #0x1F
	mov r1, #0x1A	//Modebits = 0x1A = Hypervisor Mode
	cmp r0, r1		//Vergleichen und Statusbits setzen für bedingte Sprünge

	/* Wenn Hypemode erkannt: abschalten (sprung nach _exitHyper)
	 * wenn nicht erkannt: dann weitermachen (weiterlaufen nach _bsprak)
	 */
	beq _exitHyper

	/* Qemu startet immer alle 4 Kerne
	 * Wir lassen alle anderen Kerne endless loopen
	 */
_checkCores:
	/* Id des Cpu Cores Abfragen */
	mrc p15, 0, r0, c0, c0, 5
	/* Falls nicht core 0 Core disablen */
	tst r0, #3
	bne _parkCore

/* not modeled in qemu 6.0 */
_enableAlignCheck:
	/* SCTLR des cp15 laden */
	mrc p15, 0, r0, c1, c0, 0
	/* A-bit (c1[1]) auf 1 setzen */
	orr r0, r0, #0x2
	/* neues SCTLR speichern */
	mcr p15, 0, r0, c1, c0, 0


_bsprak:
/* 	MEMORY LAYOUT
	sp_svc 0x20000 #131072
	sp_und 0x21000   +4096
	sp_usr 0x22000   +4096 
	sp_abt 0x24000   +8192
	sp_irq 0x25000   +4096
	sp_fiq 0x26000   +4096*/
		
	ldr 	sp,	=0x20000
	ldr	r0, =0x21000
	msr 	sp_und, r0
	/* user mode gets more space*/
	ldr	r0, =0x22000
	msr 	sp_usr, r0 
	ldr	r0, =0x24000
	msr 	sp_abt, r0 
	ldr	r0, =0x25000
	msr 	sp_irq, r0
	ldr	r0, =0x26000
	msr 	sp_fiq, r0

	/* set VBAR in CP15 */
	ldr r0, =_ivt
	mcr p15, 0, r0, c12, c0, 0
	
	/* change to user mode */
	//cps #0x10	//nicht in den usermode!
	cpsie i 	//erlaubt interrupts!!!
	
	/* Zu c Code springen */
	bl  start_kernel
.Lend:
	WFI
	b .Lend

_parkCore:
	/* Interrupts für Core 1 bis 3 ausschalten  */
	cpsid if
	/* In Endlosschleife springen */
	b .Lend

_exitHyper:

	/* Rücksprungadresse ins Hypervisor Rücksprungregister schreiben.
	 * Dies ist das Label _bsprak für die normale Programmausfürung.
	 */
	ldr lr, =_checkCores
	msr ELR_hyp, lr

	/* Der Hypervisor mode ist auch "nur" eine etwas kompliziertere Exception.
	 * Also springen wir aus dieser zurück.
	 * (Rücksprunge in Aufgabe 2 sind anders zu lösen!).
	 * Dazu zunächst das CPSR auslesen und die Modebits zum Supervisor ändern.
	 * Danach dies in das SPSR für den Rücksprung schreiben.
	 */
	mrs r0, cpsr
	bic r0, r0, #0x1F
	orr r0, r0, #0x13	//Supervisormode
	msr spsr_hyp, r0
	eret

#undef __ASSEMBLY__
