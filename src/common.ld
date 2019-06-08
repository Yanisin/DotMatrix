INCLUDE cortex-m-generic.ld

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
	} > rom

	/* Heap ends before the stack */
	_psp_stack = ORIGIN(ram) + LENGTH(ram) - 512;
	_psp_base = ORIGIN(ram) + LENGTH(ram) - 2048;
	__main_thread_stack_base__ = _psp_base;
	__heap_end__ = _psp_base;
	__heap_base__ = _ebss;
}
