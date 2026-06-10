	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 26, 2
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$4048, %rsp                     ## imm = 0xFD0
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movl	$0, -4020(%rbp)
	movl	%edi, -4024(%rbp)
	movq	%rsi, -4032(%rbp)
	movl	$0, -4036(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$1000, -4036(%rbp)              ## imm = 0x3E8
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movslq	-4036(%rbp), %rax
	movl	$0, -4016(%rbp,%rax,4)
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-4036(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4036(%rbp)
	jmp	LBB0_1
LBB0_4:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	cmpq	%rcx, %rax
	jne	LBB0_6
## %bb.5:
	xorl	%eax, %eax
	addq	$4048, %rsp                     ## imm = 0xFD0
	popq	%rbp
	retq
LBB0_6:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
