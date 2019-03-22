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

#include <arch/core/k1b/asm.h>

.globl _upcall_issue
.globl _upcall_issue_ret
.globl saved_sp

.section .text

/*
 * Issues an upcall.
 */
.align 8
_upcall_issue:
	_do_prologue_slow      # Save all preserved registers since
	;;                     # they may be used by the HAL.

	make $r1, saved_sp
	;;
	sw  0[$r1], $sp
	;;

	/* Switch to fake stack. */
	sw 16 + K1B_CONTEXT_SSP[$r0], $sp
	;;
	copy $sp, $r0
	;;

	/* Return from upcall*/
	goto k1b_upcall_ret
	;;

_upcall_issue_ret:

	/* Switch back to real stack. */
	make $r1, saved_sp
	;;
	lw  $bp, 0[$r1]
	;;

	_do_epilogue_slow     # Restore all preserved
	;;                    # registers.

	ret
	;;
