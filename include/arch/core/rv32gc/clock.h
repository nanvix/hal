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

#ifndef ARCH_CORE_RV32GC_CLOCK_H_
#define ARCH_CORE_RV32GC_CLOCK_H_

/**
 * @addtogroup rv32gc-clock Timer
 * @ingroup rv32gc
 *
 * @brief Programmable Timer Interface
 */
/**@{*/

	#include <arch/core/rv32gc/mcall.h>
	#include <stdint.h>

#ifndef _ASM_FILE_

	/**
	 * @brief 64-bit timer register.
	 */
	extern uint64_t *rv32gc_mtime;

	/**
	 * @brief 64-bit timer compare register.
	 */
	extern uint64_t *rv32gc_mtimecmp;

	/**
	 * @brief Initializes the clock device.
	 *
	 * @param freq     Clock frequency (in Hz).
	 * @param timebase Clock time base.
	 * @param mtime    Location of mtime register.
	 * @param mtime    Location of mtimecmp register.
	 */
	extern void rv32gc_clock_init(
		uint64_t freq,
		uint64_t timebase,
		uint64_t *mtime,
		uint64_t *mtimecmp
	);

	/**
	 * @brief Reset the clock.
	 */
	extern void rv32gc_clock_reset(void);

	/**
	 * @brief Reads the mtime register.
	 *
	 * @returns The value of the mtime register.
	 */
	static inline uint64_t rv32gc_mtime_read(void)
	{
		uint32_t lo;
		uint32_t hi;

		hi = *((uint32_t *)(rv32gc_mtime) + 1);
		lo = *((uint32_t *)(rv32gc_mtime));

		return (((hi & 0xffffffffull) << 32) | (lo & 0xffffffffull));
	}

	/**
	 * @brief Reads the mtimecmp register.
	 *
	 * @returns The value of the mtimecmp register.
	 */
	static inline uint64_t rv32gc_mtimecmp_read(void)
	{
		uint32_t lo;
		uint32_t hi;

		hi = *((uint32_t *)(rv32gc_mtimecmp) + 1);
		lo = *((uint32_t *)(rv32gc_mtimecmp));

		return (((hi & 0xffffffffull) << 32) | (lo & 0xffffffffull));
	}

	/**
	 * @brief Writes to the mtimecmp register.
	 *
	 * @param time Value to write.
	 */
	static inline void rv32gc_mtimecmp_write(uint64_t time)
	{
		*((uint32_t *)(rv32gc_mtimecmp) + 1) = 0xffffffff;
		*((uint32_t *)(rv32gc_mtimecmp)) = (uint32_t)(time & 0xffffffff);
		*((uint32_t *)(rv32gc_mtimecmp) + 1) =
			(uint32_t)((time & 0xffffffff00000000ull) >> 32);

		rv32gc_mcall_timer_ack();
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __clock_reset_fn /**< clock_reset() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see clock_reset().
	 */
	static inline void clock_reset(void)
	{
		rv32gc_clock_reset();
	}

#endif

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_CLOCK */
