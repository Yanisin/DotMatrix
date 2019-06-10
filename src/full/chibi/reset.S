.cpu cortex-m0
.fpu softvfp
.syntax unified
.thumb
.text


.thumb_func
.global reset_handler
reset_handler:
/* We use this reset handler to manually set the PSP stack and use it, which opencm3 does not do */
cpsid i



ldr r0, =_psp_stack
msr PSP, r0

/* Set SPSEL to use PSP */
movs r0, #2
msr CONTROL, r0

isb

bl crt_init
bl main
