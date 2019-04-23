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

#ifndef NANVIX_HAL_CLUSTER_CLOCK_H_
#define NANVIX_HAL_CLUSTER_CLOCK_H_

	/* Core Interface Implementation */
	#include <nanvix/hal/cluster/_cluster.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#if defined(__INTERFACE_CHECK) || defined(__INTERFACE_CHECK_CLUSTER_AL) || defined(__INTERFACE_CHECK_CLOCK)

	/* Constants */
	#ifndef INTERRUPT_CLOCK
	#error "INTERRUPT_CLOCK not defined"
	#endif

	/* Functions */
	#ifndef __clock_init_fn
	#error "clock_init() not defined?"
	#endif
	#ifndef __clock_reset_fn
	#error "clock_reset() not defined?"
	#endif

#endif

/*============================================================================*
 * Clock Device Interface                                                     *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-core-clock Clock
 * @ingroup kernel-hal-core
 *
 * @brief Clock Device Interface HAL Interface
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Initializes the clock device
	 *
	 * @param freq Frequency for the clock device.
	 */
	EXTERN void clock_init(unsigned freq);

	/**
	 * @brief Resets the timer of the clock device.
	 */
	EXTERN void clock_reset(void);

/**@}*/

#endif /* NANVIX_HAL_CLUSTER_CLOCK_H_ */

