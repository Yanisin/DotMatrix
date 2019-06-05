INCLUDE cortex-m-generic.ld

SECTIONS
{
	.applet_array : {
		. = ALIGN(4);
		__applet_array_start = .;
		KEEP(*(.applet.*));
		__applet_array_end = .;

		__worker_array_start = .;
		KEEP(*(.worker.*));
		__worker_array_end = .;
	} > rom

	/* Heap ends before the stack */
	_psp_stack = ORIGIN(ram) + LENGTH(ram) - 256;
	__heap_end__ = ORIGIN(ram) + LENGTH(ram) - 512;
	__heap_base__ = _ebss;
}
