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

#ifndef ARCH_CORE_MOR1KX_CLOCK_H_
#define ARCH_CORE_MOR1KX_CLOCK_H_

/**
 * @addtogroup or1k-core-clock Clock
 * @ingroup or1k-core
 *
 * @brief Integrated Clock Device
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Estimated CPU frequency (in Hz), 50Mhz/30Hz.
	 */
	#define OR1K_CPU_FREQUENCY 1666666

	/**
	 * @brief Initializes the clock driver in the or1k architecture.
	 *
	 * @param freq Target frequency for the clock device.
	 */
	EXTERN void or1k_clock_init(unsigned freq);

	/**
	 * @brief Resets the clock device.
	 */
	EXTERN void or1k_clock_reset(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond mor1kx
 */

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __clock_init_fn  /**< clock_init()  */
	#define __clock_reset_fn /**< clock_reset() */
	/**@}*/

	/**
	 * @see or1k_clock_init().
	 */
	static inline void clock_init(unsigned freq)
	{
		or1k_clock_init(freq);
	}

	/**
	 * @see or1k_clock_reset().
	 */
	static inline void clock_reset(void)
	{
		or1k_clock_reset();
	}

/**@endcond*/

#endif /* ARCH_CORE_MOR1KX_CLOCK */
