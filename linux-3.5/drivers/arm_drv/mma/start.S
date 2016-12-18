
.text
.align 2
.global millet9527 

millet9527:
ST:
	b reset
	nop		
	nop		
	nop	  
	nop	  
	nop	  
	b irq 			

reset:	
	stmfd sp!, {r0-r12, lr}
	bl _clr_bss
	bl main
	ldmfd sp!, {r0-r12, pc}
irq:
	mov sp,#0x73000000
	sub lr,lr,#4
	stmfd sp!, {r0-r12, lr}
	mov lr, pc
	ldr pc, _do_irq
	ldmfd sp!, {r0-r12, pc}^

_do_irq:
	.word do_irq

END:

.global vectors_start
.global vectors_end 
vectors_end:
	.word END 
vectors_start:
	.word ST

.globl __bss_start
__bss_start:
	.word _bss_start
.globl __bss_end
__bss_end:
	.word _bss_end
	

_clr_bss:
	ldr r0, __bss_start
	ldr r1, __bss_end
	mov r2, #0x00000000
_millet:
	str r2, [r0], #4
	cmp r0, r1
	blt _millet
	mov pc, lr
iic_millet__:
	.word 0x9527
