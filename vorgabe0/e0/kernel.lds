ENTRY(_start)
SECTIONS
{
	. = 0x00008000;
	.init : { *(.init) }
	
	. = 0x000100000;
	.text :{ 
	build/arch/*(.text .text.*)
	build/cpu/*(.text .text.*)
	build/lib/*(.text .text.*)
	build/kernel/*(.text .text.*)
	}

	. = 0x000200000;
	.rodata :{ 
	build/arch/*(.rodata .rodata.*)
	build/cpu/*(.rodata .rodata.*)
	build/lib/*(.rodata .rodata.*)
	build/kernel/*(.rodata .rodata.*)
	}

	. = 0x000300000;
	.data :{ 
	build/arch/*(.data .data.*)
	build/cpu/*(.data .data.*)
	build/lib/*(.data .data.*)
	build/kernel/*(.data .data.*)
	}

	. = 0x000400000;
	.bss :{ 
	build/arch/*(.bss .bss.*)
	build/cpu/*(.bss .bss.*)
	build/lib/*(.bss .bss.*)
	build/kernel/*(.bss .bss.*)
	}

	. = 0x000500000;
	.user_text :{ 
	build/user/*(.text .text.*)
	build/user/user_lib/*(.text .text.*)
	}

	. = 0x000600000;
	.user_rodata :{
	build/user/*(.rodata .rodata.*)
	build/user/user_lib/*(.rodata .rodata.*)
	}

	. = 0x000700000;
	.user_data :{ 
	build/user/*(.data .data.*)
	build/user/user_lib/*(.data .data.*)
	}

	. = 0x00800000;
	.user_bss :{ 
	build/user/*(.bss .bss.*)
	build/user/user_lib/*(.bss .bss.*)
	}
}

