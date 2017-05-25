	.file   PasTest.pas
	.comm a,4,4
	.comm b,4,4
	.comm c,4,4
	.comm x,4,4
	.comm y,4,4
	.comm z,4,4
	.comm w,8,8
	.comm t,8,8
	.comm v,8,8
	.comm p,1,1
	.comm q,1,1
	.text
	.globl  main
	.type main, @function
main:
.LFB0:
	pushq %rbp
	movq  %rsp, %rbp
	push, %rax
	movl $3, %eax
	push, %rax
	movl $4, %eax
	addl %eax, (%rsp)
	movl %(%rsp), %eax
	addq $8, %rsp
	movl %eax,  c
	movl %eax,  b
	movl %eax,  a
	push, %rax
	movl a, %eax
	push, %rax
	movl c, %eax
	mull %eax, (%rsp)
	movl (%rsp), %eax
	addq $8, %rsp
	push, %rax
	movl b, %eax
	cvtsi2ss %eax, %eax
	movss %eax,  y
	cvtsi2ss (%rsp), %xmm1
	movss %eax, %xmm0
	addss %xmm1, %xmm0
	movss %xmm0, %eax
	addq $8, %rsp
	movss %eax,  x
	push, %rax
	movb $1, %al
	movb %al,  q
	movb %al,  p
	push, %rax
	movss x, %eax
	push, %rax
	movss a, %eax
	cvtsi2ss %eax, %eax
	movss (%rsp), %xmm1
	movss %eax, %xmm0
	mulss %xmm1, %xmm0
	movss %xmm0, %eax
	addq $8, %rsp
	push, %rax
	movss b, %eax
	cvtsi2ss %eax, %eax
	movss (%rsp), %xmm1
	movss %eax, %xmm0
	mulss %xmm1, %xmm0
	movss %xmm0, %eax
	addq $8, %rsp
	push, %rax
	movss c, %eax
	cvtsi2ss %eax, %eax
	movss (%rsp), %xmm1
	movss %eax, %xmm0
	mulss %xmm1, %xmm0
	movss %xmm0, %eax
	addq $8, %rsp
	push, %rax
	movss y, %eax
	movss (%rsp), %xmm1
	movss %eax, %xmm0
	mulss %xmm1, %xmm0
	movss %xmm0, %eax
	addq $8, %rsp
	push, %rax
	movsd w, %rax
	cvtss2sd (%rsp), %xmm1
	movsd %rax, %xmm0
	mulsd %xmm1, %xmm0
	movsd %xmm0, %rax
	addq $8, %rsp
	movsd %rax, w
	movl $0, %rax
	popq	%rbp
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
