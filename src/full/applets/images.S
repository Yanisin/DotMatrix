.section .rodata

.macro INCBIN name file
.align 4
\name\()_start:
.incbin "\file"
\name\()_end:
.align 4
.global \name
\name:
.long \name\()_start
.long \name\()_end
.endm

INCBIN img_sysgo applets/img_sysgo.pgm
INCBIN img_banner applets/img_banner.pgm

