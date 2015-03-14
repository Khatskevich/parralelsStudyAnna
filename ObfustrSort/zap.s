	.file	"zap.c"
	.text
	.globl	ShellsSort
	.type	ShellsSort, @function
ShellsSort:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	-28(%rbp), %eax
	shrl	%eax
	movl	%eax, -8(%rbp)
	jmp	.L2
.L10:
	movl	-8(%rbp), %eax
	movl	%eax, -16(%rbp)
	jmp	.L3
.L9:
	movl	-16(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	.L4
.L8:
	movl	-8(%rbp), %eax
	movl	-12(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpl	-4(%rbp), %eax
	jle	.L5
	movl	-12(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	movl	-12(%rbp), %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, %eax
	leaq	0(,%rax,4), %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	movl	-8(%rbp), %eax
	subl	%eax, -12(%rbp)
	jmp	.L4
.L5:
	jmp	.L7
.L4:
	movl	-12(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jae	.L8
.L7:
	movl	-12(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -16(%rbp)
.L3:
	movl	-16(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jb	.L9
	movl	-8(%rbp), %eax
	shrl	%eax
	movl	%eax, -8(%rbp)
.L2:
	cmpl	$0, -8(%rbp)
	jne	.L10
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	ShellsSort, .-ShellsSort
	.section	.rodata
.LC0:
	.string	"%d "
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	$4, -32(%rbp)
	movl	$677, -28(%rbp)
	movl	$23, -24(%rbp)
	movl	$64, -20(%rbp)
	movl	$34, -16(%rbp)
	movl	$4, -12(%rbp)
	leaq	-32(%rbp), %rax
	movl	$6, %esi
	movq	%rax, %rdi
	call	ShellsSort
	movl	$0, -36(%rbp)
	jmp	.L12
.L13:
	movl	-36(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	addl	$1, -36(%rbp)
.L12:
	cmpl	$5, -36(%rbp)
	jle	.L13
	movl	$10, %edi
	call	putchar
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
