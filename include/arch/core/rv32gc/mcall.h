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

#ifndef ARCH_CORE_RV32GC_MCALL_H_
#define ARCH_CORE_RV32GC_MCALL_H_

/**
 * @addtogroup rv32gc-core-mcall Machine Call
 * @ingroup rv32gc-core
 *
 * @brief Machine Call Interface
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CORE_TYPES

	#include <arch/core/rv32gc/types.h>

	/**
	 * @name Machine Calls
	 */
	/**@{*/
	#define RV32GC_MCALL_CSR_READ       1 /**< rv32gc_mcall_csr_read()    */
	#define RV32GC_MCALL_CSR_SET        2 /**< rv32gc_mcall_csr_set()     */
	#define RV32GC_MCALL_CSR_CLEAR      3 /**< rv32gc_mcall_csr_clear()   */
	#define RV32GC_MCALL_TIMER_ACK      4 /**< rv32gc_mcall_timer_ack()   */
	#define RV32GC_MCALL_MINT_ENABLE    5 /**< rv32gc_mcall_int_enable()  */
	#define RV32GC_MCALL_MINT_DISABLE   6 /**< rv32gc_mcall_int_disable() */
	#define RV32GC_MCALL_IPI_ACK        7 /**< rv32gc_mcall_ipi_ack()     */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Issues a machine call with no argument.
	 *
	 * @param mcall_nr Machine call number.
	 *
	 * @returns The machine call return value.
	 */
	static inline rv32gc_word_t rv32gc_mcall0(
		rv32gc_word_t mcall_nr)
	{
		register rv32gc_word_t _mcall_nr __asm__("a0") = mcall_nr;
		register rv32gc_word_t ret __asm__ ("a0");

		__asm__ __volatile__ (
			"ecall"
			: "=r" (ret)
			: "r"  (_mcall_nr)
			: "memory", "cc"
		);

		return (ret);
	}

	/**
	 * @brief Issues a machine call with one argument.
	 *
	 * @param mcall_nr Machine call number.
	 * @param arg0     Argument 0.
	 *
	 * @returns The machine call return value.
	 */
	static inline rv32gc_word_t rv32gc_mcall1(
		rv32gc_word_t mcall_nr,
		rv32gc_word_t arg0)
	{
		register rv32gc_word_t _mcall_nr __asm__("a0") = mcall_nr;
		register rv32gc_word_t _arg0 __asm__("a1") = arg0;
		register rv32gc_word_t ret __asm__ ("a0");

		__asm__ __volatile__ (
			"ecall"
			: "=r" (ret)
			: "r"  (_mcall_nr),
			  "r"  (_arg0)
			: "memory", "cc"
		);

		return (ret);
	}

	/**
	 * @brief Issues a machine call with two argument.
	 *
	 * @param mcall_nr Machine call number.
	 * @param arg0     Argument 0.
	 * @param arg1     Argument 1.
	 *
	 * @returns The machine call return value.
	 */
	static inline rv32gc_word_t rv32gc_mcall2(
		rv32gc_word_t mcall_nr,
		rv32gc_word_t arg0,
		rv32gc_word_t arg1)
	{
		register rv32gc_word_t _mcall_nr __asm__("a0") = mcall_nr;
		register rv32gc_word_t _arg0 __asm__("a1") = arg0;
		register rv32gc_word_t _arg1 __asm__("a2") = arg1;
		register rv32gc_word_t ret __asm__ ("a0");

		__asm__ __volatile__ (
			"ecall"
			: "=r" (ret)
			: "r"  (_mcall_nr),
			  "r"  (_arg0),
			  "r"  (_arg1)
			: "memory", "cc"
		);

		return (ret);
	}

	/**
	 * @brief Reads a CSR register.
	 *
	 * @param csr Target CSR register.
	 *
	 * @returns The value of the @p csr register.
	 */
	static inline rv32gc_word_t rv32gc_mcall_csr_read(rv32gc_word_t csr)
	{
		return (rv32gc_mcall1(RV32GC_MCALL_CSR_READ, csr));
	}

	/**
	 * @brief Sets a bit in a CSR register.
	 *
	 * @param csr Target CSR register.
	 * @param bit Target bit
	 */
	static inline void rv32gc_mcall_csr_set(rv32gc_word_t csr, rv32gc_word_t bit)
	{
		rv32gc_mcall2(RV32GC_MCALL_CSR_SET, csr, bit);
	}

	/**
	 * @brief Clears a bit in a CSR register.
	 *
	 * @param csr Target CSR register.
	 * @param bit Target bit
	 */
	static inline void rv32gc_mcall_csr_clear(rv32gc_word_t csr, rv32gc_word_t bit)
	{
		rv32gc_mcall2(RV32GC_MCALL_CSR_CLEAR, csr, bit);
	}

	/**
	 * @brief Enables machine interrupts.
	 */
	static inline void rv32gc_mcall_mint_enable(void)
	{
		rv32gc_mcall0(RV32GC_MCALL_MINT_ENABLE);
	}

	/**
	 * @brief Disables machine interrupts.
	 */
	static inline void rv32gc_mcall_mint_disable(void)
	{
		rv32gc_mcall0(RV32GC_MCALL_MINT_DISABLE);
	}

	/**
	 * @brief Acknowledges a timer interrupt.
	 */
	static inline void rv32gc_mcall_timer_ack(void)
	{
		rv32gc_mcall0(RV32GC_MCALL_TIMER_ACK);
	}

	/**
	 * @brief Acknowledges an IPI.
	 */
	static inline void rv32gc_mcall_ipi_ack(void)
	{
		rv32gc_mcall0(RV32GC_MCALL_IPI_ACK);
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32GC_MCALL_H_  */
