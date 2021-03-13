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

#include <arch/core/arm64-core/types.h>
#include <arch/core/arm64-core/asm.h>

/* Exported symbols. */
.global _arm64_cluster_core_reset

.section .text

/*===========================================================================*
 * _arm64_cluster_core_reset()                                             *
 *===========================================================================*/

/*
 * Resets the underlying core
 */
.align ARM64_WORD_SIZE
_arm64_cluster_core_reset:

	arm64core_clear_gprs

	/* Get the core ID*/
    mrs	x0, mpidr_el1
	and	x0, x0, #3

	/* Reset stack. */
	arm64_core_stack_reset x0

	/* Restart core. */
	bl arm64_cluster_slave_setup
	b halt

/* Infinite loop */
halt: wfe
      b halt