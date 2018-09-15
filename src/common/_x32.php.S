#if defined(CPU32)

SETGPU_LoadRealFunction_x32:
		FRAME_POINTER_START
		subl	$32, %esp

		movl	RealDll, %eax
		cmpl	$0, %eax
		jne		LoadReal_DllIsLoaded

		subl $4, %esp
		leal	REAL_DLL_PATH, %eax
		movl	%eax, (%esp)
		call	*__imp__LoadLibraryA@4
		
		# TODO better error handling
		movl	%eax, RealDll
		pushl	%eax
		call	*__imp__SETGPU_on_real_dll_loaded
		addl $4, %esp
		movl	RealDll, %eax

	LoadReal_DllIsLoaded:
		movl %eax, %ebx
		movl 8(%ebp), %eax

		pushl %eax
		pushl %ebx
		call	*__imp__GetProcAddress@8

		addl	$32, %esp
		FRAME_POINTER_END
		ret



<? foreach($all_functions as $function) { ?>


	.global _<?=$function?>

	_<?=$function?>:
			SAVE_REGS_ALIGN16_X32
			leal	.str_<?= $function ?>, %eax
			pushl %eax
			call	*__imp__puts
			addl $4, %esp
			RESTORE_REGS_ALIGN16_X32
			# int $3

			movl	Real_<?=$function?>, %eax
			cmpl	$0, %eax
			je		2f


		1:
			<? if (in_array($function, $fake_functions)) { ?>
				leal Fake_<?=$function?>, %eax
				jmp		*%eax
			<? } else { ?>
				jmp		*%eax
			<? } ?>
		   # (unreachable)

		2:
			# rax is confirmed zero, need to load Real_*

			leal	.str_<?= $function ?>, %eax

			# int $3
			SAVE_REGS_ALIGN16_X32
			pushl %eax
			call	SETGPU_LoadRealFunction_x32
			addl	$4, %esp
			RESTORE_REGS_ALIGN16_X32
			# int $3

			movl	%eax, Real_<?=$function?>

			jmp		1b
<? } ?>

#endif

