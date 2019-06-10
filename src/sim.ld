SECTIONS
{
	.applet_array : {
		. = ALIGN(4);
		__applet_array_start = .;
		KEEP(*(.applet.*));
		__applet_array_end = .;

		__dminit_array_start = .;
		KEEP(*(.dminit.*));
		__dminit_array_end = .;
	}
}

__heap_base__ = static_heap;
__heap_end__ = __heap_base__ + 0x10000;

INSERT AFTER .rodata;

