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

#ifndef ARCH_CORE_RI5CY_REGS_H_
#define ARCH_CORE_RI5CY_REGS_H_

	#ifndef __NEED_CORE_REGS
		#error "do not include this file"
	#endif

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_CORE_MREGS

/**
 * @addtogroup ri5cy-core-regs CSRs
 * @ingroup ri5cy-core
 *
 * @brief Control and Status Registers (CSRs)
 */
/**@{*/

	#include <arch/core/ri5cy/types.h>
	#include <arch/core/rv32gc/mregs.h>

	/**
	 * @brief Getter for register field.
	 *
	 * @param FIELD Target register field
	 * @param x     Register value.
	 */
	#define __RI5CY_REG_GET(FIELD,x) \
		(((x) & RI5CY_ ## FIELD ## _MASK) >> RI5CY_ ## FIELD ## _SHIFT)

	/**
	 * @brief Setter for register field.
	 *
	 * @param FIELD Target register field
	 * @param x     Register value.
	 */
	#define __RI5CY_REG_SET(FIELD,x, val) \
		((x) | (((val) << RI5CY_ ## FIELD ## _SHIFT) & RI5CY_ ## FIELD ## _MASK))

	/**
	 * @name Bit Shifts to MHARTID Register
	 */
	/**@{*/
	#define RI5CY_MHARTID_COREID_SHIFT    0 /**< Core ID    */
	#define RI5CY_MHARTID_CLUSTERID_SHIFT 5 /**< Cluster ID */
	/**@}*/

	/**
	 * @name Masks for MHARTID Register
	 */
	/**@{*/
	#define RI5CY_MHARTID_COREID_MASK    (0x1f << RI5CY_MHARTID_COREID_SHIFT)    /**< Core ID    */
	#define RI5CY_MHARTID_CLUSTERID_MASK (0x3f << RI5CY_MHARTID_CLUSTERID_SHIFT) /**< Cluster ID */
	/**@}*/

	/**
	 * @name Getters for MHARTID Register
	 */
	/**@{*/
	#define RI5CY_MHARTID_COREID(x)    __RI5CY_REG_GET(MHARTID_COREID, x)    /**< Core ID    */
	#define RI5CY_MHARTID_CLUSTERID(x) __RI5CY_REG_GET(MHARTID_CLUSTERID, x) /**< Cluster ID */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Aliases to Functions
	 */
	/**@{*/
	#define ri5cy_mstatus_read() rv32gc_mstatus_read()  /**< @ref rv32gc_mstatus_read() */
	#define ri5cy_mtvec_read()   rv32gc_mtvec_read()    /**< @ref rv32gc_mtvec_read()   */
	#define ri5cy_mepc_read()    rv32gc_mepc_read()     /**< @ref rv32gc_mepc_read()    */
	#define ri5cy_mcause_read()  rv32gc_mcause_read()   /**< @ref rv32gc_mcause_read()  */
	#define ri5cy_mhartid_read() rv32gc_mhartid_read()  /**< @ref rv32gc_mhartid_read() */
	/**@}*/

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RI5CY_REGS_H_ */

