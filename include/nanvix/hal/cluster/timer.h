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

#ifndef NANVIX_HAL_CLUSTER_TIMER_H_
#define NANVIX_HAL_CLUSTER_TIMER_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_TIMER)

	/* Constants */
	#ifndef INTERRUPT_TIMER
	#error "INTERRUPT_TIMER not defined"
	#endif

	/* Functions */
	#ifndef __timer_init_fn
	#error "timer_init() not defined?"
	#endif
	#ifndef __timer_reset_fn
	#error "timer_reset() not defined?"
	#endif
	#ifndef __clock_read_fn
	#error "clock_read() not defined?"
	#endif

#endif

/*============================================================================*
 * Timer Device Interface                                                     *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-timer Timer
 * @ingroup kernel-hal-core
 *
 * @brief Timer Device Interface HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>
	#include <stdint.h>

	/**
	 * @brief Simulated clock for architectures that do not
	 * have Real Time Clock (RTC).
	 */
	EXTERN uint64_t timer_value;

	/**
	 * @brief Initializes the timer device.
	 *
	 * @param freq Frequency for the timer device.
	 */
	EXTERN void timer_init(unsigned freq);

#ifdef __NANVIX_HAL

	/**
	 * @brief Resets the timer of the timer device.
	 */
	EXTERN void timer_reset(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Returns the clock value.
	 *
	 * If the current architecture have an appropriate Real
	 * Time Clock, exports the function, otherwise, uses the
	 * counter provided by the HAL.
	 */
#if (CLUSTER_HAS_RTC)
	EXTERN uint64_t clock_read(void);
#else
	static inline uint64_t clock_read(void)
	{
		return (timer_value);
	}
#endif

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_TIMER_H_ */
