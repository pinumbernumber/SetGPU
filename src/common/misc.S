#pragma once


#if defined(CPU64)
#define GLOBL_LABEL(name) .globl name; name
#elif defined(CPU32)
#define GLOBL_LABEL(name) .globl name; name
# # define GLOBL_LABEL(name) .globl _##name; _##name
#endif



#if defined(CPU64)

# #define SAVE_REGS \
# 	# pushq	%rax	;\
# 	pushq	%rbx	;\
# 	pushq	%rcx	;\
# 	pushq	%rdx	;\
# 	pushq	%r8		;\
# 	pushq	%r9		;\
# 	pushq	%r10	;\
# 	pushq	%r11

# #define RESTORE_REGS \
# 	popq	%r11	;\
# 	popq	%r10	;\
# 	popq	%r9		;\
# 	popq	%r8		;\
# 	popq	%rdx	;\
# 	popq	%rcx	;\
# 	popq	%rbx	;\
# 	# popq	%rax


#define FOO 32
.macro SAVE_REGS
	# pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%r8	
	pushq	%r9	
	pushq	%r10
	pushq	%r11
	# subq	$FOO, %rsp
.endm

.macro RESTORE_REGS
	# addq	$FOO, %rsp
	popq	%r11
	popq	%r10
	popq	%r9	
	popq	%r8	
	popq	%rdx
	popq	%rcx
	popq	%rbx
	# popq	%rax
.endm

.macro SAVE_REGS_ALIGN16
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%r8	
	pushq	%r9	
	pushq	%r10
	pushq	%r11
	subq	$8, %rsp
.endm

.macro RESTORE_REGS_ALIGN16
	addq	$8, %rsp
	popq	%r11
	popq	%r10
	popq	%r9	
	popq	%r8	
	popq	%rdx
	popq	%rcx
	popq	%rbx
.endm


#define SP_REGISTER %rsp
#define BP_REGISTER %rbp

#define PUSH_NATIVE pushq
#define POP_NATIVE popq
#define MOV_NATIVE movq


#elif defined(CPU32)

.macro SAVE_REGS_ALIGN16_X32
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi
	pushl %ebp
	subl	$8, %esp
.endm

.macro RESTORE_REGS_ALIGN16_X32
	addl	$8, %esp
	popl %ebp
	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx
.endm

#define SP_REGISTER %esp
#define BP_REGISTER %ebp

#define PUSH_NATIVE pushl
#define POP_NATIVE popl
#define MOV_NATIVE movl


#else
#error Unknown CPU
#endif


.macro FRAME_POINTER_START
	PUSH_NATIVE	BP_REGISTER
	MOV_NATIVE	SP_REGISTER, BP_REGISTER
.endm

.macro FRAME_POINTER_END
	MOV_NATIVE	BP_REGISTER, SP_REGISTER
	POP_NATIVE	BP_REGISTER
.endm


#if defined(CPU64)

#define LONG_OR_QUAD .quad
#define AX %rax

#elif defined(CPU32)

#define LONG_OR_QUAD .long
#define AX %eax

#else
#error Unknown CPU
#endif