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

#include <nanvix/hal/cluster/memory.h>
#include <arch/cluster/arm64-cluster/cores.h>
#include <arch/cluster/arm64-cluster/memory.h>
#include <nanvix/const.h>

/**
 * @brief Memory layout.
 */
PUBLIC struct memory_region mem_layout[MEM_REGIONS] = {
	{
		.pbase = 0,
		.vbase = 0,
		.pend  = 0,
		.vend  = 0,
		.size  = 0,
		.writable = true,
		.executable = true,
		.root_pgtab_num = 0,
		.desc = "kernel"
	}
};


/*============================================================================*
 * arm64_mmu_setup()                                                           *
 *============================================================================*/

/**
 * The arm_mmu_setup() function initializes the Memory Management Unit
 * (MMU) of the underlying arm64 core.
 */
PUBLIC int arm64_mmu_setup(void) {
	asm ("msr TTBR0_EL1, X0" ::);	// Set TTBR0
	asm ("msr TTBR1_EL1, X1" ::);	// Set TTBR1
  	asm ("msr TCR_EL1, X2" ::);		// Set TCR
  	isb();							// The ISB forces these changes to be seen before the MMU is enabled.
  	asm("msr X0, SCTLR_EL1" ::);	// Read System Control Register configuration data
  	asm("orr X0, X0, #1" ::);		// Set [M] bit and enable the MMU.
  	asm("msr SCTLR_EL1, X0" ::);	// Write System Control Register configuration data
  	isb();							// The ISB forces these changes to be seen by the next instruction
	return 0;
}
