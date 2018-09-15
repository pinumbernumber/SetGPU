#if defined(CPU64)

FatalError:
	leaq	.FatalErrorMsg(%rip), %rcx
	call	puts
	call	abort
	ret # ?

.macro FatalError_if_ax_0
	cmpq	$0,	%rax
	jne		.+7			# skip over 2-byte jump + 5 byte call
	call	FatalError
.endm

SETGPU_LoadRealFunction_x64_preserve_regs_no_home:
		FRAME_POINTER_START
		SAVE_REGS_ALIGN16
		# TODO save flags?

		# locals
		subq	$16, %rsp
		# home for called
		subq	$32, %rsp

	# Test whether the real DLL handle is loaded
	# and load it if not
		movq	RealDll(%rip), %rax
		cmpq	$0, %rax
		jne		LoadReal_DllIsLoaded
	# NeedToLoadLibrary:
		movq	%rcx, -56(%rbp)
		# movq	%rcx, Asdf(%rip)
		leaq	REAL_DLL_PATH(%rip), %rcx
		call	*__imp_LoadLibraryA(%rip)
		FatalError_if_ax_0

	LoadLibraryOK:
		movq	%rax, RealDll(%rip)
	# Call setgpu init
		movq	%rax, %rcx
		call	*__imp_SETGPU_on_real_dll_loaded(%rip)
		movq	RealDll(%rip), %rax


	# restore rcx (func name string)
		movq	-56(%rbp), %rcx

	# Load the actual function
	LoadReal_DllIsLoaded:
		movq	%rcx, %rdx
		movq	%rax, %rcx
		call	*__imp_GetProcAddress(%rip)
		FatalError_if_ax_0
	GetProcAddressOK:
		addq	$32, %rsp
		addq	$16, %rsp
		# addq	$8, %rsp
		RESTORE_REGS_ALIGN16
		FRAME_POINTER_END
		ret

<? foreach($all_functions as $function) { ?>

	.globl <?=$function?>

	<?=$function?>:
			pushq %rax
			SAVE_REGS_ALIGN16
			subq $32, %rsp
			leaq	.str_<?= $function ?>(%rip), %rcx
			call puts
			addq $32, %rsp
			RESTORE_REGS_ALIGN16
			popq %rax

			movq	Real_<?=$function?>(%rip), %rax
			cmpq	$0, %rax
			je		2f


		1:
			# rax is confirmed nonzero; Real_* is loaded
			<? if (in_array($function, $fake_functions)) { ?>
				leaq Fake_<?=$function?>(%rip), %rax
				jmp		*%rax
			<? } else { ?>
				jmp		*%rax
			<? } ?>
		   # (unreachable)

		2:
			# rax is confirmed zero, need to load Real_*
			# backup rcx (also usefully aligns stack mod 16)

			pushq	%rcx
			leaq	.str_<?= $function ?>(%rip), %rcx
			call	SETGPU_LoadRealFunction_x64_preserve_regs_no_home
			popq	%rcx

			# We have the pointer, store it for future use
			# and jump back to the relevant code above
			movq	%rax, Real_<?=$function?>(%rip)
			jmp		1b

<? } ?>

#endif
