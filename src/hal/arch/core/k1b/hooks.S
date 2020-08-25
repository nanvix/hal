/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Must come first. */
#define _ASM_FILE_
#define __NEED_CORE_TYPES
#define __NEED_CORE_CONTEXT

#include <arch/core/k1b/types.h>
#include <arch/core/k1b/ctx.h>
#include <arch/core/k1b/excp.h>
#include <arch/core/k1b/asm.h>
#include <arch/core/k1b/upcall.h>
#include <arch/core/k1b/mOS.h>

.global _k1b_do_kcall
.global _k1b_do_excp
.global _k1b_do_int

.section .text

/*===========================================================================*
 * _scoreboard_get()                                                         *
 *===========================================================================*/

/*
 * Gets the scoreboard of the underlying core.
 */
.macro _scoreboard_get s0 s1

	/* Get core ID. */
	get   \s1, $pi
	;;
	extfz \s1, \s1, 15, 11
	;;

	/* Get pointer to scoreboard. */
	make \s0 = _scoreboard_start
	;;
	sll  \s1, \s1, MOS_SCOREBOARD_PER_CPU_ELEMENT_SHIFT
	;;
	add  \s0, \s0, \s1
	;;

.endm

/*
 * Gets exception/interrupt cause (28: exception cause in PS register)
 */
.macro _get_suspension_cause from to tmp
	lw  \tmp = MOS_VC_REG_PS[\from]
	;;
	srl \to = \tmp, 28
	;;
.endm

/*===========================================================================*
 * _k1b_mos_context_save()                                                   *
 *===========================================================================*/

/*
 * Save mOS specific register to currently stack pointer.
 */
.macro _k1b_mos_context_save from to tmp

	/* Save Processing Status. */
	lw \tmp = MOS_VC_REG_PS[\from]
	;;
	sw K1B_CONTEXT_PS[\to], \tmp
	;;

	/* Save Shadow Processing Status */
	lw \tmp = MOS_VC_REG_SPS[\from]
	;;
	sw K1B_CONTEXT_SPS[\to], \tmp
	;;

	/* Save Shadow Shadow Processing Status */
	lw \tmp = MOS_VC_REG_SSPS[\from]
	;;
	sw K1B_CONTEXT_SSPS[\to], \tmp
	;;

	/* Save Shadow Shadow Shadow Processing Status */
	lw \tmp = MOS_VC_REG_SSSPS[\from]
	;;
	sw K1B_CONTEXT_SSSPS[\to], \tmp
	;;

	/* Save Shadown Program Counter. */
	lw \tmp = MOS_VC_REG_SPC[\from]
	;;
	sw K1B_CONTEXT_SPC[\to], \tmp
	;;

	/* Save Shadown Shadown Program Counter. */
	lw \tmp = MOS_VC_REG_SSPC[\from]
	;;
	sw K1B_CONTEXT_SSPC[\to], \tmp
	;;

	/* Save Shadown Shadown Shadown Program Counter. */
	lw \tmp = MOS_VC_REG_SSSPC[\from]
	;;
	sw K1B_CONTEXT_SSSPC[\to], \tmp
	;;

	/* Save Shadow Stack Pointer. */
	lw \tmp = MOS_VC_REG_SSP[\from]
	;;
	sw K1B_CONTEXT_SSP[\to], \tmp
	;;

	/* Save Shadow Shadow Stack Pointer. */
	lw \tmp = MOS_VC_REG_SSSP[\from]
	;;
	sw K1B_CONTEXT_SSSP[\to], \tmp
	;;

	/* Save Shadow Shadow Shadow Stack Pointer. */
	lw \tmp = MOS_VC_REG_SSSSP[\from]
	;;
	sw K1B_CONTEXT_SSSSP[\to], \tmp
	;;

.endm

/*===========================================================================*
 * _k1b_mos_context_restore()                                                *
 *===========================================================================*/

/*
 * Restore mOS specific register from currently stack pointer.
 */
.macro _k1b_mos_context_restore from to tmp


	/* Restore Shadow Shadow Shadow Stack Pointer. */
	lw \tmp = K1B_CONTEXT_SSSSP[\from]
	;;
	sw MOS_VC_REG_SSSSP[\to], \tmp
	;;

	/* Restore Shadow Shadow Stack Pointer. */
	lw \tmp = K1B_CONTEXT_SSSP[\from]
	;;
	sw MOS_VC_REG_SSSP[\to], \tmp
	;;

	/* Restore Shadow Stack Pointer. */
	lw \tmp = K1B_CONTEXT_SSP[\from]
	;;
	sw MOS_VC_REG_SSP[\to], \tmp
	;;

	/* Restore Program Counter. */
	lw \tmp = K1B_CONTEXT_SSSPC[\from]
	;;
	sw MOS_VC_REG_SSSPC[\to], \tmp
	;;

	/* Restore Program Counter. */
	lw \tmp = K1B_CONTEXT_SSPC[\from]
	;;
	sw MOS_VC_REG_SSPC[\to], \tmp
	;;

	/* Restore program counter. */
	lw $r4 = K1B_CONTEXT_SPC[\from]
	;;
	sw MOS_VC_REG_SPC[\to], \tmp
	;;

	/* Restore Shadow Shadow Shadow Processing Status */
	lw \tmp = K1B_CONTEXT_SSSPS[\from]
	;;
	sw MOS_VC_REG_SSSPS[\to], \tmp
	;;

	/* Restore Shadow Shadow Processing Status */
	lw \tmp = K1B_CONTEXT_SSPS[\from]
	;;
	sw MOS_VC_REG_SSPS[\to], \tmp
	;;

	/* Restore Shadow Processing Status */
	lw \tmp = K1B_CONTEXT_SPS[\from]
	;;
	sw MOS_VC_REG_SPS[\to], \tmp
	;;

	/**
	 * Restore Processing Status
	 * @todo Is it Necessary? It is will be overwritten.
	 **/
	lw \tmp = K1B_CONTEXT_PS[\from]
	;;
	sw MOS_VC_REG_PS[\to], \tmp
	;;

.endm

/*===========================================================================*
 * _do_kcall()                                                               *
 *===========================================================================*/

/*
 * System call hook.
 */
.align 8
_k1b_do_kcall:

	cb.nez $r7, _do_kcall.continue
	;;

		copy $sp, $r1
		;;

		_scoreboard_get $r2 $r3
		;;

		/* Restore saved execution mos context. */
		_k1b_mos_context_restore $sp $r2 $r4
		;;

		/* Restore saved execution context. */
		_k1b_context_restore
		;;

		goto _do_kcall.out
		;;

	_do_kcall.continue:

		/* Save preserved registers. */
		_do_prologue
		;;

		/* mOS supports trap calls with 8 arguments, and thus the trap
		 * call number is stored in the R7.  However, we support trap
		 * calls only with 5 arguments, so we should fix the arguments
		 * of the trap call here, by storing into the register R5 the
		 * trap call number.
		 */
		copy $r5, $r7
		;;

		/*
		 * Invoke system call dispatcher.
		 */
		_redzone_alloc
		;;
		call do_kcall
		;;
		_redzone_free
		;;

		/* Restore preserved registers. */
		_do_epilogue
		;;

	_do_kcall.out:

		scall MOS_VC_RFE
		;;
		ret
		;;

/*===========================================================================*
 * _do_excp()                                                                *
 *===========================================================================*/

/*
 * Exception hook.
 */
.align 8
_k1b_do_excp:

	/*
	 * Save execution context
	 * in the current stack.
	 */
	_k1b_context_save
	;;

	/*
	 * Save a reference to execution context
	 * into r1. The high-level exception handler
	 * dispatcher will need it.
	 */
	copy $r1 = $sp
	;;

	_scoreboard_get $r2 $r3
	;;

	/*
	 * Save execution mos context
	 * in the current stack.
	 */
	_k1b_mos_context_save $r2 $r1 $r4
	;;

	/*
	 * Save exception information
	 * in the current stack.
	 */
	add  $sp, $sp, -K1B_EXCP_SIZE
	;;
	_get_suspension_cause $r2 $r4 $r5  /* Exception code. */
	;;
	sw  K1B_EXCP_NR[$sp], $r4
	;;
	lw  $r4 = MOS_VC_REG_EA[$r2]      /* Exception address. */
	;;
	sw K1B_EXCP_EA[$sp], $r4
	;;
	lw  $r4 = MOS_VC_REG_SPC[$r2]     /* Instruction address. */
	;;
	sw K1B_EXCP_SPC[$sp], $r4
	;;

	/*
	 * Save a reference to exception information
	 * into r0. The high-level exception handler
	 * dispatcher will need it.
	 */
	copy $r0 = $sp
	;;

	/*
	 * Call exception dispatcher.
	 */
	_redzone_alloc
	;;
	call do_exception
	;;
	_redzone_free
	;;

	/*
	 * Wipe out exception information
	 * from current stack.
	 */
	add $sp, $sp, K1B_EXCP_SIZE
	;;

	_scoreboard_get $r2 $r3
	;;

	/* Restore saved execution mos context. */
	_k1b_mos_context_restore $sp $r2 $r4
	;;

	/*
	 * Restore saved execution context.
	 */
	_k1b_context_restore
	;;

	/* Restore exception context. */
	scall MOS_VC_RFE
	;;
	ret
	;;

/*===========================================================================*
 * _do_int()                                                                 *
 *===========================================================================*/

/*
 * Interrupt hook.
 */
.align 8
_k1b_do_int:

	/*
	 * Save execution context
	 * in the current stack.
	 */
	_k1b_context_save
	;;

	/*
	 * Save a reference to execution context
	 * into r1. The high-level exception handler
	 * dispatcher will need it.
	 */
	copy $r1 = $sp
	;;

	_scoreboard_get $r2 $r3
	;;

	/*
	 * Save execution mos context
	 * in the current stack.
	 */
	_k1b_mos_context_save $r2 $r1 $r4
	;;

	/*
	 * Save interrupt number into r0.
	 */
	_get_suspension_cause $r2 $r0 $r5  /* Interrupt code. */
	;;

	/*
	 * Call interrupt dispatcher.
	 */
	_redzone_alloc
	;;
	call __k1b_do_int
	;;
	_redzone_free
	;;

	_scoreboard_get $r2 $r3
	;;

	/* Restore saved execution mos context. */
	_k1b_mos_context_restore $sp $r2 $r4
	;;

	/*
	 * Restore saved execution context.
	 */
	_k1b_context_restore
	;;

	/* Restore exception context. */
	scall MOS_VC_RFE
	;;
	ret
	;;
