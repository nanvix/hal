/*
 * MIT License
 *
 * Copyright(c) 2011-2019 The Maintainers of Nanvix
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

.section .text

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

/*===========================================================================*
 * _do_kcall()                                                             *
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

		/* Restore saved program counter. */
		lw  $r4 = K1B_CONTEXT_SPC[$sp]
		;;
		sw  MOS_VC_REG_SPC[$r2], $r4
		;;

		/* Restore shadow stack pointer. */
		lw  $r4 = K1B_CONTEXT_SSP[$sp]
		;;
		sw  MOS_VC_REG_SSP[$r2], $r4
		;;

		/* Restore saved execution context. */
		k1b_context_restore
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
		redzone_alloc
		;;
		call do_kcall
		;;
		redzone_free
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
	k1b_context_save
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

	/* Processing Status. */
	lw  $r4 = MOS_VC_REG_PS[$r2]
	;;
	sw  K1B_CONTEXT_PS[$r1], $r4
	;;

	/* Saved Processing Status */
	lw  $r4 = MOS_VC_REG_SPS[$r2]
	;;
	sw  K1B_CONTEXT_SPS[$r1], $r4
	;;

	/* Saved Program Counter. */
	lw  $r4 = MOS_VC_REG_SPC[$r2]
	;;
	sw  K1B_CONTEXT_SPC[$r1], $r4
	;;

	/* Shadow Stack Pointer. */
	lw  $r4 = MOS_VC_REG_SSP[$r2]
	;;
	sw  K1B_CONTEXT_SSP[$r1], $r4
	;;

	/* Shadow Shadow Stack Pointer. */
	lw  $r4 = MOS_VC_REG_SSSP[$r2]
	;;
	sw  K1B_CONTEXT_SSSP[$r1], $r4
	;;

	/* Shadow Shadow Shadow Stack Pointer. */
	lw  $r4 = MOS_VC_REG_SSSSP[$r2]
	;;
	sw  K1B_CONTEXT_SSSSP[$r1], $r4
	;;

	/*
	 * Save exception information
	 * in the current stack.
	 */
	add  $sp, $sp, -K1B_EXCP_SIZE
	;;
	lw  $r5 = MOS_VC_REG_PS[$r2]
	;;
	srl $r4 = $r5, 28                 /* Exception code. */
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
	redzone_alloc
	;;
	call do_exception
	;;
	redzone_free
	;;

	/*
	 * Wipe out exception information
	 * from current stack.
	 */
	add $sp, $sp, K1B_EXCP_SIZE
	;;

	_scoreboard_get $r2 $r3
	;;

	/* Restore saved program counter. */
	lw  $r4 = K1B_CONTEXT_SPC[$sp]
	;;
	sw  MOS_VC_REG_SPC[$r2], $r4
	;;

	/* Restore shadow stack pointer. */
	lw  $r4 = K1B_CONTEXT_SSP[$sp]
	;;
	sw  MOS_VC_REG_SSP[$r2], $r4
	;;

	/*
	 * Restore saved execution context.
	 */
	k1b_context_restore
	;;

	/* Restore exception context. */
	scall MOS_VC_RFE
	;;
	ret
	;;
