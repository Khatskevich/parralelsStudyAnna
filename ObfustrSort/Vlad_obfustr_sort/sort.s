	.file	"sort.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$3944, %rsp
	movq	%rdi, -4056(%rbp)
	movq	%rsi, -4064(%rbp)
	movl	$0, -4044(%rbp)
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4012(%rbp)
	leaq	-4000(%rbp), %rsi
	movl	$0, %eax
	movl	$500, %edx
	movq	%rsi, %rdi
	movq	%rdx, %rcx
	rep stosq
	movl	$0, -4040(%rbp)
	movl	-4012(%rbp), %eax
	movl	%eax, -4044(%rbp)
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	subl	$1, %eax
	movl	%eax, -4048(%rbp)
	jmp	.L2
.L13:
	movl	-4048(%rbp), %eax
	movl	%eax, -4032(%rbp)
	movl	$0, -4040(%rbp)
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	addl	$1, %eax
	movl	%eax, -4028(%rbp)
	jmp	.L3
.L12:
	movl	-4032(%rbp), %eax
	addl	$1, %eax
	leal	(%rax,%rax), %edx
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jne	.L4
	movl	$0, -4024(%rbp)
	movl	$0, -4024(%rbp)
	jmp	.L5
.L6:
	movq	-4064(%rbp), %rax
	movl	(%rax), %edx
	movl	-4024(%rbp), %eax
	cltq
	movl	%edx, -4000(%rbp,%rax,4)
	addl	$1, -4024(%rbp)
.L5:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	cmpl	-4024(%rbp), %eax
	jg	.L6
	movl	-4028(%rbp), %eax
	movl	%eax, -4036(%rbp)
	jmp	.L7
.L4:
	movl	-4028(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-4028(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L8
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	addl	$1, %eax
	movl	%eax, -4036(%rbp)
	jmp	.L7
.L8:
	movl	-4032(%rbp), %eax
	addl	$1, %eax
	addl	%eax, %eax
	movl	%eax, -4036(%rbp)
.L7:
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jge	.L9
	cmpl	$999, -4048(%rbp)
	jg	.L10
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -4008(%rbp)
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4008(%rbp), %eax
	movl	%eax, (%rdx)
	movl	-4036(%rbp), %eax
	movl	%eax, -4032(%rbp)
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	addl	$1, %eax
	movl	%eax, -4028(%rbp)
	jmp	.L3
.L10:
	jmp	.L11
.L9:
	movl	$1, -4040(%rbp)
.L3:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	cmpl	-4028(%rbp), %eax
	jle	.L11
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	cmpl	-4044(%rbp), %eax
	jl	.L11
	cmpl	$0, -4040(%rbp)
	je	.L12
.L11:
	subl	$1, -4044(%rbp)
	subl	$1, -4048(%rbp)
.L2:
	cmpl	$0, -4048(%rbp)
	jns	.L13
	movl	$0, -4048(%rbp)
	jmp	.L14
.L21:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	addl	%eax, %eax
	addl	$1, %eax
	movl	%eax, -4044(%rbp)
	movl	-4048(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	leal	-1(%rax), %edx
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	jge	.L15
	cmpl	$0, -4044(%rbp)
	js	.L15
	movl	-4048(%rbp), %eax
	cltq
	movl	-4000(%rbp,%rax,4), %eax
	movl	%eax, -4044(%rbp)
	movl	-4048(%rbp), %eax
	leal	1(%rax), %ecx
	movl	-4048(%rbp), %eax
	cltq
	movl	-4000(%rbp,%rax,4), %edx
	movslq	%ecx, %rax
	movl	%edx, -4000(%rbp,%rax,4)
	movl	-4048(%rbp), %eax
	cltq
	movl	-4044(%rbp), %edx
	movl	%edx, -4000(%rbp,%rax,4)
	movl	-4048(%rbp), %eax
	leal	2(%rax), %ecx
	movl	-4048(%rbp), %eax
	cltq
	addq	$2, %rax
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movslq	%ecx, %rax
	movl	%edx, -4000(%rbp,%rax,4)
	jmp	.L16
.L15:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	cmpl	-4048(%rbp), %eax
	jle	.L17
	cmpl	$0, -4044(%rbp)
	jns	.L18
.L17:
	movl	-4048(%rbp), %eax
	leal	1(%rax), %ecx
	movl	-4048(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movslq	%ecx, %rax
	movl	%edx, -4000(%rbp,%rax,4)
	jmp	.L16
.L18:
	movl	$0, -4020(%rbp)
	movl	$0, -4020(%rbp)
	jmp	.L19
.L20:
	movl	-4020(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4020(%rbp), %eax
	cltq
	movl	-4000(%rbp,%rax,4), %eax
	movl	%eax, (%rdx)
	addl	$1, -4020(%rbp)
.L19:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	cmpl	-4020(%rbp), %eax
	jg	.L20
.L16:
	addl	$1, -4048(%rbp)
.L14:
	movq	-4064(%rbp), %rax
	movl	(%rax), %eax
	subl	$1, %eax
	cmpl	-4048(%rbp), %eax
	jg	.L21
	jmp	.L22
.L30:
	movq	-4056(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4004(%rbp)
	movl	-4048(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-4056(%rbp), %rax
	movl	%edx, (%rax)
	movl	-4048(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4004(%rbp), %eax
	movl	%eax, (%rdx)
	movl	$0, -4040(%rbp)
	movl	$0, -4032(%rbp)
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	movl	%eax, -4016(%rbp)
	jmp	.L23
.L29:
	movl	-4016(%rbp), %eax
	leal	1(%rax), %edx
	movl	-4048(%rbp), %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	jne	.L24
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	addl	$1, %eax
	movl	%eax, -4036(%rbp)
	jmp	.L25
.L24:
	movl	-4016(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-4016(%rbp), %eax
	cltq
	addq	$2, %rax
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L26
	movl	-4016(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4036(%rbp)
	jmp	.L25
.L26:
	movl	-4016(%rbp), %eax
	addl	$2, %eax
	movl	%eax, -4036(%rbp)
.L25:
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jge	.L27
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -4008(%rbp)
	movl	-4032(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-4056(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	movl	-4036(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-4056(%rbp), %rax
	addq	%rax, %rdx
	movl	-4008(%rbp), %eax
	movl	%eax, (%rdx)
	movl	-4036(%rbp), %eax
	movl	%eax, -4032(%rbp)
	movl	-4032(%rbp), %eax
	addl	%eax, %eax
	movl	%eax, -4016(%rbp)
	jmp	.L23
.L27:
	movl	$1, -4040(%rbp)
.L23:
	movl	-4016(%rbp), %eax
	addl	$1, %eax
	cmpl	-4048(%rbp), %eax
	jge	.L28
	cmpl	$0, -4040(%rbp)
	je	.L29
.L28:
	subl	$1, -4048(%rbp)
.L22:
	cmpl	$0, -4048(%rbp)
	jg	.L30
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	sort, .-sort
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
