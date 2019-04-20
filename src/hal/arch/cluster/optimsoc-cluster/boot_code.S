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
#define __NEED_CLUSTER_OPTIMSOC

#include <arch/cluster/optimsoc-cluster.h>

/* Exported symbols. */
.globl start
.globl core.kstack

/*============================================================================*
 *                              bootstrap section                             *
 *============================================================================*/

.section .bootstrap,"ax",@progbits

/*----------------------------------------------------------------------------*
 * start()                                                                    *
 *----------------------------------------------------------------------------*/

/*
 * Kernel entry point.
 */
start:

	or1k_clear_gprs

	/* Supervisor mode. */
	l.ori	r1, r0, 0x1
	l.mtspr	r0, r1, OR1K_SPR_SR

	/* Reset stack. */
	l.mfspr r3, r0, OR1K_SPR_COREID
	or1k_core_stack_reset r3

	l.mfspr r3, r0, OR1K_SPR_COREID
	l.sfeqi r3, 0
	l.bf start.master
	l.nop

	/* Slave. */
	start.slave:
		l.jal optimsoc_cluster_slave_setup
		l.nop
		halt

	/* Master. */
	start.master:
		l.jal optimsoc_cluster_master_setup
		l.nop
		halt
