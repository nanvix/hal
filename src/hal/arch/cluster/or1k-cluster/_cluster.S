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
#define __NEED_OR1K_REGS

#include <arch/core/or1k/asm.h>
#include <arch/core/or1k/core.h>
#include <arch/core/or1k/mmu.h>

/* Exported symbols. */
.global _or1k_core_reset
.global core.kstack

.section .text

/*===========================================================================*
 * _or1k_core_reset()                                                        *
 *===========================================================================*/

/*
 * Resets the underlying core.
 */
_or1k_core_reset:
	/* Clear GPRs. */
	or1k_clear_gprs

	/* Get core id. */
	OR1K_LOAD_SYMBOL_2_GPR(r1, core.kstack)
	l.mfspr r3, r0, OR1K_SPR_COREID
	l.addi  r3, r3, 1

	/* Setup kernel stack. */
	l.slli  r3, r3, OR1K_PAGE_SHIFT
	l.add   r1, r1, r3
	l.addi  r1, r1, -OR1K_WORD_SIZE  /* Stack pointer. */
	l.or    r2, r1, r0               /* Frame pointer. */

	/* Restart core. */
	l.j or1k_slave_setup
	l.nop

	/* Never gets here. */
	_or1k_core_reset.halt:
		l.j _or1k_core_reset.halt
		l.nop
