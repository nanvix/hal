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

#ifndef ARCH_CLUSTER_WOLFE_CTRL_H_
#define ARCH_CLUSTER_WOLFE_CTRL_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/wolfe/_wolfe.h>

	#ifndef __NEED_WOLFE_CTRL
	#error "wolfe uart device not required"
	#endif

/**
 * @addtogroup wolfe-cluster-ctrl CTRL Unit
 * @ingroup wolfe-cluster
 *
 * @brief Cluster Control Unit
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @name Control Unit Registers
	 */
	/**@{*/
	#define WOLFE_CTRL_CORESTATUS 0xa0 /**< EoC and Chip Status Register */
	/**@}*/

	/**
	 * @name Bit Shifts for CORESTATUS Register
	 */
	/**@{*/
	#define WOLFE_CTRL_CORESTATUS_STATUS_SHIFT  0 /**< Chip Status        */
	#define WOLFE_CTRL_CORESTATUS_EOC_SHIFT    31 /**< End of Computation */
	/**@}*/

	/**
	 * @name Masks for CORESTATUS Register
	 */
	/**@{*/
	#define WOLFE_CTRL_CORESTATUS_STATUS_MASK (0x7fffffff << WOLFE_CTRL_CORESTATUS_EOC_SHIFT) /**< Chip Status        */
	#define WOLFE_CTRL_CORESTATUS_EOC_MASK    (1 << WOLFE_CTRL_CORESTATUS_EOC_SHIFT)          /**< End of Computation */
	/**@}*/

	/**
	 * @name EoC and Chip Status Register (CORESTATUS)
	 */
	/**@{*/
	#define WOLFE_CTRL_CORESTATUS_EOC_EN (WOLFE_CTRL_CORESTATUS_EOC_MASK) /**< End of Computation */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Powersoff the underlying cluster.
	 */
	static inline void wolfe_ctrl_corestatus_write(paddr_t ctrl_base)
	{
		asm volatile (
			"sw    %1, 0(%0)"
			:
			: "r" (ctrl_base + WOLFE_CTRL_CORESTATUS),
			  "r" (WOLFE_CTRL_CORESTATUS_EOC_EN)
		);
	}

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* ARCH_CLUSTER_WOLFE_CTRL_H_ */
