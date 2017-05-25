	.file	"flthack.c"
	.comm	xx,8,8
	.comm	i,4,4
	.comm	x,4,4
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$i, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, x(%rip)
	movsd	.LC2(%rip), %xmm0
	movsd	%xmm0, xx(%rip)
	movl	$3, i(%rip)
	movsd	xx(%rip), %xmm1
	movsd	.LC3(%rip), %xmm0
	mulsd	%xmm0, %xmm1
	movl	i(%rip), %eax
	pxor	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	addsd	%xmm1, %xmm0
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, x(%rip)
	movl	i(%rip), %eax
	pxor	%xmm0, %xmm0
	cvtsi2ss	%eax, %xmm0
	movss	x(%rip), %xmm1
	addss	%xmm1, %xmm0
	movss	x(%rip), %xmm1
	ucomiss	%xmm0, %xmm1
	jbe	.L7
	movss	.LC4(%rip), %xmm0
	movss	%xmm0, x(%rip)
	jmp	.L4
.L7:
	movss	.LC5(%rip), %xmm0
	movss	%xmm0, x(%rip)
.L4:
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC1:
	.long	1078529622
	.align 8
.LC2:
	.long	3229815407
	.long	1074340298
	.align 8
.LC3:
	.long	1374389535
	.long	1074339512
	.align 4
.LC4:
	.long	1073741824
	.align 4
.LC5:
	.long	1077936128
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
