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
#define __NEED_CORE_TYPES

#include <arch/core/or1k/core.h>
#include <arch/core/or1k/mmu.h>
#include <arch/core/or1k/types.h>
#include <arch/core/or1k/asm.h>

/* Exported symbols. */
.global _or1k_cluster_core_reset
.global core.kstack

.section .text

/*===========================================================================*
 * _or1k_cluster_core_reset()                                                *
 *===========================================================================*/

/*
 * Resets the underlying core.
 */
_or1k_cluster_core_reset:

	/* Save coreid. */
	OR1K_EXCEPTION_STORE_GPR3

	/* Clear GPRs. */
	or1k_clear_gprs

	/* Reset stack. */
	OR1K_EXCEPTION_LOAD_GPR3(r3)
	or1k_core_stack_reset r3

	/* Restart core. */
	l.j or1k_cluster_slave_setup
	l.nop
	halt
