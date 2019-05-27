SECTIONS
{
	.applet_array : {
		. = ALIGN(4);
		__applet_array_start = .;
		KEEP(*(.applet.*));
		__applet_array_end = .;
	}
}

INSERT AFTER .rodata;
