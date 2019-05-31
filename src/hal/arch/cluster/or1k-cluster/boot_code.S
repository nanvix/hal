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

#if (defined(__or1k_cluster__))
	#include <arch/cluster/or1k-cluster/cores.h>
#elif (defined(__optimsoc_cluster__))
	#include <arch/cluster/optimsoc-cluster/cores.h>
#endif

/* Exported symbols. */
.globl start
.globl kstacks
.globl or1k_icache_setup
.globl or1k_dcache_setup

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

	/* Enable Instruction Cache. */
	l.jal or1k_icache_setup
	l.nop

	/* Enable Data Cache. */
	l.jal or1k_dcache_setup
	l.nop

	/* Reset stack. */
	l.mfspr r3, r0, OR1K_SPR_COREID
	or1k_core_stack_reset r3

	l.mfspr r3, r0, OR1K_SPR_COREID
	l.sfeqi r3, 0
	l.bf start.master
	l.nop

	/* Slave. */
	start.slave:
		l.jal or1k_cluster_slave_setup
		l.nop
		halt

	/* Master. */
	start.master:
		l.jal or1k_cluster_master_setup
		l.nop
		halt

/*----------------------------------------------------------------------------*
 *                           or1k_icache_setup                                *
 *----------------------------------------------------------------------------*/

or1k_icache_setup:
	/* Check if IC present and skip enabling otherwise */
	l.mfspr r24, r0,  OR1K_SPR_UPR
	l.andi  r26, r24, OR1K_SPR_UPR_ICP
	l.sfeq  r26, r0
	l.bf	9f
	l.nop

	/* Disable IC */
	l.mfspr r6, r0, OR1K_SPR_SR
	l.addi  r5, r0, -1
	l.xori  r5, r5, OR1K_SPR_SR_ICE
	l.and   r5, r6, r5
	l.mtspr r0, r5, OR1K_SPR_SR

	/* 
	 * Establish cache block size
	 * If BS=0, 16;
	 * If BS=1, 32;
	 * r14 contain block size
	 */
	l.mfspr r24, r0,  OR1K_SPR_ICCFGR
	l.andi	r26, r24, OR1K_SPR_ICCFGR_CBS
	l.srli	r28, r26, 7
	l.ori	r30, r0,  16
	l.sll	r14, r30, r28

	/*
	 * Establish number of cache sets
	 * r16 contains number of cache sets
	 * r28 contains log(# of cache sets)
	 */
	l.andi  r26, r24, OR1K_SPR_ICCFGR_NCS
	l.srli 	r28, r26, 3
	l.ori   r30, r0,  1
	l.sll   r16, r30, r28

	/* Invalidate IC */
	l.addi  r6, r0,  0
	l.sll   r5, r14, r28

1:
	l.mtspr r0, r6, OR1K_SPR_ICBIR
	l.sfne  r6, r5
	l.bf    1b
	l.add   r6, r6, r14

	/* Enable IC */
	l.mfspr r6, r0, OR1K_SPR_SR
	l.ori   r6, r6, OR1K_SPR_SR_ICE
	l.mtspr r0, r6, OR1K_SPR_SR
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
	l.nop
9:
	l.jr    r9
	l.nop

/*----------------------------------------------------------------------------*
 *                           or1k_dcache_setup                                *
 *----------------------------------------------------------------------------*/

or1k_dcache_setup:
	/* Check if DC present and skip enabling otherwise */
	l.mfspr r24, r0,  OR1K_SPR_UPR
	l.andi  r26, r24, OR1K_SPR_UPR_DCP
	l.sfeq  r26, r0
	l.bf	9f
	l.nop

	/* Disable DC */
	l.mfspr r6, r0, OR1K_SPR_SR
	l.addi  r5, r0, -1
	l.xori  r5, r5, OR1K_SPR_SR_DCE
	l.and   r5, r6, r5
	l.mtspr r0, r5, OR1K_SPR_SR

	/*
	 * Establish cache block size
	 * If BS=0, 16;
	 * If BS=1, 32;
	 * r14 contain block size
	 */
	l.mfspr r24, r0,  OR1K_SPR_DCCFGR
	l.andi	r26, r24, OR1K_SPR_DCCFGR_CBS
	l.srli	r28, r26, 7
	l.ori	r30, r0,  16
	l.sll	r14, r30, r28

	/*
	 * Establish number of cache sets
	 * r16 contains number of cache sets
	 * r28 contains log(# of cache sets)
	 */
	l.andi  r26, r24, OR1K_SPR_DCCFGR_NCS
	l.srli 	r28, r26, 3
	l.ori   r30, r0,  1
	l.sll   r16, r30, r28

	/* Invalidate DC */
	l.addi  r6, r0,  0
	l.sll   r5, r14, r28
1:
	l.mtspr r0, r6, OR1K_SPR_DCBIR
	l.sfne  r6, r5
	l.bf    1b
	l.add   r6, r6, r14

	/* Enable DC */
	l.mfspr r6, r0, OR1K_SPR_SR
	l.ori   r6, r6, OR1K_SPR_SR_DCE
	l.mtspr r0, r6, OR1K_SPR_SR
9:
	l.jr    r9
	l.nop
