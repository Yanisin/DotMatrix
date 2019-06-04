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
	}
}

INSERT AFTER .rodata;
