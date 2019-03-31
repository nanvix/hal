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

#ifndef ARCH_CORE_RV32I_REGS_H_
#define ARCH_CORE_RV32I_REGS_H_

	#ifndef __NEED_CORE_REGS
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32i-core-regs Registers
 * @ingroup rv32i-core
 *
 * @brief Registers
 */
/**@{*/

	/**
	 * @name Machine Cause Register (mcause)
	 */
	/**@{*/
	#define RV32I_MCAUSE_INT   0x80000000ul /**< Interrupt? */
	#define RV32I_MCAUSE_CAUSE 0x000000fful /**< Trap Cause */
	/**@}*/

	/**
	 * @name Machine Status Register (mstatus)
	 */
	/**@{*/
	#define RV32I_MSTATUS_UIE  (0x1ul <<  0) /**< User Interrupt Enable                */
	#define RV32I_MSTATUS_SIE  (0x1ul <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_MSTATUS_MIE  (0x1ul <<  3) /**< Machine Interrupt Enable             */
	#define RV32I_MSTATUS_UPIE (0x1ul <<  4) /**< User Previous Interrupt Enable       */
	#define RV32I_MSTATUS_SPIE (0x1ul <<  5) /**< Supervisor Previous Interrupt Enable */
	#define RV32I_MSTATUS_MPIE (0x1ul <<  7) /**< Machine Previous Interrupt Enable    */
	#define RV32I_MSTATUS_SPP  (0x1ul <<  8) /**< Supervisor Previous Priviledge       */
	#define RV32I_MSTATUS_MPP  (0x3ul << 11) /**< Machine Previous Priviledge          */
	/**@}*/

	/**
	 * @name Machine Interrupt Enable Register (mie)
	 */
	/**@{*/
	#define RV32I_MIE_USIE (0x1ul <<  0) /**< User Software Interrupt Enable       */
	#define RV32I_MIE_SSIE (0x1ul <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_MIE_R0   (0x1ul <<  2) /**< Reserved                             */
	#define RV32I_MIE_MSIE (0x1ul <<  3) /**< Machine Software Interrupt Enable    */
	#define RV32I_MIE_UTIE (0x1ul <<  4) /**< User Timer Interrupt Enable          */
	#define RV32I_MIE_STIE (0x1ul <<  5) /**< Supervisor Timer Interrupt Enable    */
	#define RV32I_MIE_R1   (0x1ul <<  6) /**< Reserved                             */
	#define RV32I_MIE_MTIE (0x1ul <<  7) /**< Machine Timer Interrupt Enable       */
	#define RV32I_MIE_UEUE (0x1ul <<  8) /**< User External Interrupt Enable       */
	#define RV32I_MIE_SEIE (0x1ul <<  9) /**< Supervisor External Interrupt Enable */
	#define RV32I_MIE_R2   (0x1ul << 10) /**< Reserved                             */
	#define RV32I_MIE_MEIE (0x1ul << 11) /**< Machine External Interrupt Enable    */
	/**@}*/

/**@}*/

#endif /* ARCH_CORE_RV32I_REGS_H_ */
