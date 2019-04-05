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

#ifndef ARCH_CORE_RV32I_CLOCK_H_
#define ARCH_CORE_RV32I_CLOCK_H_

/**
 * @addtogroup clock-core-timer Timer
 * @ingroup clock-core
 *
 * @brief Programmable Timer Interface
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CORE_PIC
	#define __NEED_CORE_TYPES

	#include <arch/core/rv32i/mcall.h>
	#include <arch/core/rv32i/pic.h>
	#include <arch/core/rv32i/types.h>

	/**
	 * @brief Clock frequency (10 MHz)
	 */
	#define MTIME_TIMEBASE 10000000

	/**
	 * @brief Address of the mtime register.
	 */
	#define MTIME_ADDR (PIC_BASE + PIC_MTIME_OFFSET)

	/**
	 * @brief Address of the mtimecmp register.
	 */
	#define MTIMECMP_ADDR (PIC_BASE + PIC_MTIMECMP_OFFSET)

#ifndef _ASM_FILE_

	/**
	 * @brief Initializes the clock device.
	 *
	 * @param freq Clock frequency (in Hz).
	 */
	extern void rv32i_clock_init(unsigned freq);

	/**
	 * @brief Reset the clock.
	 */
	extern void rv32i_clock_reset(void);

	/**
	 * @brief Reads the mtime register.
	 *
	 * @returns The value of the mtime register.
	 */
	static inline rv32i_dword_t rv32i_mtime_read(void)
	{
		rv32i_word_t lo;
		rv32i_word_t hi;

		hi = *(RV32I_WORD_PTR(MTIME_ADDR + RV32I_WORD_SIZE));
		lo = *(RV32I_WORD_PTR(MTIME_ADDR));

		return (RV32I_MAKE_DWORD(hi, lo));
	}

	/**
	 * @brief Reads the mtimecmp register.
	 *
	 * @returns The value of the mtimecmp register.
	 */
	static inline rv32i_dword_t rv32i_mtimecmp_read(void)
	{
		rv32i_word_t lo, hi;

		hi = *(RV32I_WORD_PTR(MTIMECMP_ADDR + RV32I_WORD_SIZE));
		lo = *(RV32I_WORD_PTR(MTIMECMP_ADDR));

		return (RV32I_MAKE_DWORD(hi, lo));
	}

	/**
	 * @brief Writes to the mtimecmp register.
	 *
	 * @param time Value to write.
	 */
	static inline void rv32i_mtimecmp_write(rv32i_dword_t time)
	{
		*((rv32i_word_t *)(MTIMECMP_ADDR + RV32I_WORD_SIZE)) = -1;
		*((rv32i_word_t *)(MTIMECMP_ADDR)) = RV32I_WORD(time & -1);
		*((rv32i_word_t *)(MTIMECMP_ADDR + RV32I_WORD_SIZE)) =
			RV32I_WORD(time >> RV32I_WORD_BIT);

		rv32i_mcall_timer_ack();
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __clock_init_fn   /**< clock_init() */
	#define __clock_reset_fn /**< clock_reset() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see clock_init().
	 */
	static inline void clock_init(unsigned freq)
	{
		rv32i_clock_init(freq);
	}

	/**
	 * @see clock_reset().
	 */
	static inline void clock_reset(void)
	{
		rv32i_clock_reset();
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32I_CLOCK */
