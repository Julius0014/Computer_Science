	.file	"ssim.c"
	.section	.rodata
.LC0:
	.string	"FORK ERROR"
	.text
	.globl	funct1
	.type	funct1, @function
funct1:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1072, %rsp
	movl	%edi, -1060(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -1052(%rbp)
	movl	$0, -1048(%rbp)
	jmp	.L2
.L8:
	call	fork
	movl	%eax, -1044(%rbp)
	movl	-1044(%rbp), %eax
	cmpl	$-1, %eax
	je	.L4
	testl	%eax, %eax
	je	.L5
	jmp	.L3
.L4:
	movl	$.LC0, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L5:
	movl	-1052(%rbp), %eax
	movl	%eax, -1048(%rbp)
	nop
.L3:
	cmpl	$0, -1044(%rbp)
	jne	.L6
	jmp	.L7
.L6:
	addl	$1, -1052(%rbp)
.L2:
	movl	-1052(%rbp), %eax
	cmpl	-1060(%rbp), %eax
	jle	.L8
.L7:
	movl	-1048(%rbp), %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L10
	call	__stack_chk_fail
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	funct1, .-funct1
	.section	.rodata
.LC1:
	.string	"pipe error"
.LC2:
	.string	"process%i"
.LC3:
	.string	"WRITE ERROR"
	.align 8
.LC4:
	.string	"process%i received a message from %s\n"
.LC5:
	.string	"READ ERROR"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$3240, %rsp
	.cfi_offset 3, -24
	movl	%edi, -3236(%rbp)
	movq	%rsi, -3248(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$0, -3224(%rbp)
	jmp	.L12
.L14:
	leaq	-3184(%rbp), %rax
	movl	-3224(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	pipe
	cmpl	$-1, %eax
	jne	.L13
	movl	$.LC1, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L13:
	addl	$1, -3224(%rbp)
.L12:
	cmpl	$9, -3224(%rbp)
	jle	.L14
	movl	$9, %edi
	call	funct1
	movl	%eax, -3212(%rbp)
	movl	$1, -3220(%rbp)
	movl	$0, -3220(%rbp)
	jmp	.L15
.L17:
	movl	-3220(%rbp), %eax
	cmpl	-3212(%rbp), %eax
	je	.L16
	movl	-3220(%rbp), %eax
	cltq
	movl	-3184(%rbp,%rax,8), %eax
	movl	%eax, %edi
	call	close
.L16:
	addl	$1, -3220(%rbp)
.L15:
	cmpl	$8, -3220(%rbp)
	jle	.L17
	movl	-3212(%rbp), %eax
	imull	$5972261, %eax, %eax
	movl	%eax, %edi
	call	srand
	movl	$214748364, -3208(%rbp)
	movl	-3208(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, -3208(%rbp)
	movl	$0, -3216(%rbp)
	jmp	.L18
.L19:
	call	rand
	movl	%eax, -3204(%rbp)
	movl	-3204(%rbp), %ecx
	movl	$1717986919, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -3200(%rbp)
	movl	-3204(%rbp), %eax
	cmpl	-3208(%rbp), %eax
	jge	.L19
	movl	-3212(%rbp), %eax
	cmpl	-3200(%rbp), %eax
	je	.L19
	movl	-3212(%rbp), %edx
	leaq	-3104(%rbp), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf
	movl	%eax, -3224(%rbp)
	movl	-3224(%rbp), %eax
	addl	$1, %eax
	movslq	%eax, %rdx
	movl	-3200(%rbp), %eax
	cltq
	movl	-3180(%rbp,%rax,8), %eax
	leaq	-3104(%rbp), %rcx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write
	shrq	$63, %rax
	movzbl	%al, %eax
	movl	%eax, -3196(%rbp)
	cmpl	$0, -3196(%rbp)
	je	.L20
	movl	$.LC3, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L20:
	addl	$1, -3216(%rbp)
.L18:
	cmpl	$11, -3216(%rbp)
	jle	.L19
	movl	$0, -3220(%rbp)
	jmp	.L21
.L22:
	movl	-3220(%rbp), %eax
	cltq
	movl	-3180(%rbp,%rax,8), %eax
	movl	%eax, %edi
	call	close
	addl	$1, -3220(%rbp)
.L21:
	cmpl	$9, -3220(%rbp)
	jle	.L22
	jmp	.L23
.L24:
	leaq	-2080(%rbp), %rcx
	movl	-3212(%rbp), %edx
	leaq	-1056(%rbp), %rax
	movl	$.LC4, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf
	movl	%eax, -3188(%rbp)
	movl	-3188(%rbp), %eax
	movslq	%eax, %rdx
	leaq	-1056(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write
.L23:
	movl	-3224(%rbp), %eax
	addl	$1, %eax
	movslq	%eax, %rdx
	movl	-3212(%rbp), %eax
	cltq
	movl	-3184(%rbp,%rax,8), %eax
	leaq	-2080(%rbp), %rcx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read
	movl	%eax, -3192(%rbp)
	movl	-3224(%rbp), %eax
	addl	$1, %eax
	cmpl	%eax, -3192(%rbp)
	je	.L24
	cmpl	$0, -3192(%rbp)
	jns	.L25
	movl	$.LC5, %edi
	call	perror
	movl	$-1, %edi
	call	exit
.L25:
	movl	-3212(%rbp), %eax
	cltq
	movl	-3184(%rbp,%rax,8), %eax
	movl	%eax, %edi
	call	close
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L26
	call	__stack_chk_fail
.L26:
	addq	$3240, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
