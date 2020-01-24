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

#include <arch/core/k1b/asm.h>
#include <arch/core/k1b/upcall.h>
#include <arch/core/k1b/types.h>

.global k1b_upcall_ret

.section .text

/*===========================================================================*
 * k1b_upcall_ret()                                                          *
 *===========================================================================*/

/*
 * Returns from an upcall.
 */
.align 8
k1b_upcall_ret:

	/* Do upcall. */
	lw  $r1, 0[$sp]               # Target Function
	;;
	add $r0, $sp, 2*K1B_WORD_SIZE # Argument
	;;

	redzone_alloc
	;;
	icall $r1
	;;
	redzone_free
	;;

	/* Wipe out upcall stack frame. */
	lw  $r0, 4[$sp]                # arg
	;;
	add $r0, $r0, 2*K1B_WORD_SIZE  # argsize + fn
	;;
	add $sp, $sp, $r0
	;;

	/* Restore old context. */
	copy $r1, $sp
	;;
	make $r0, K1B_NR_upcall_ret
	;;
	scall $r0
	;;
