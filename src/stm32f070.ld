
/* Define memory regions. */
MEMORY
{
	/* First 8K is reserved for DDFU */
	rom (rx) : ORIGIN = 0x08004000, LENGTH = 128K - 16K
	/* And we reserve 0x100 for interrupt vector table */
	ram (rwx) : ORIGIN = 0x20000100, LENGTH = 16K - 0x100
}

/* Include the common ld script. */
INCLUDE common.ld



