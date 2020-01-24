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

#ifndef PROCESSOR_LINUX64_H_
#define PROCESSOR_LINUX64_H_

	#ifndef __NEED_PROCESSOR_LINUX64
		#error "bad processor configuration?"
	#endif

	/* Processor Interface Implementation */
	#include <arch/processor/linux64/_linux64.h>

/**
 * @addtogroup processor Linux64
 * @ingroup processors
 *
 * @brief Linux64 Processor
 */
/**@*/

	#include <arch/processor/linux64/clusters.h>
	#include <arch/processor/linux64/noc.h>
	#include <nanvix/const.h>

#ifdef __NANVIX_HAL

	/**
	 * @brief Powers on the underlying processor.
	 *
	 * @param nclusters Number of clusters to power on.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
	EXTERN int linux64_processor_boot(int nclusters);

	/**
	 * @brief Initializes the underlying processor.
	 */
	EXTERN void linux64_processor_setup(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Powers off the underlying processor.
	 */
	EXTERN NORETURN void linux64_processor_poweroff(void);

/**@}*/

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_processor
 */

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define PROCESSOR_IS_MULTICLUSTER 1 /**< Multicluster feature */
	#define PROCESSOR_HAS_NOC         1 /**< NoC feature          */
	/**@}*/

	/**
	 * @brief Provided Functions
	 */
	/**@{*/
	#define __processor_poweroff_fn /**< processor_poweroff() */
	/**@}*/

	/**
	 * @see linux64_processor_poweroff().
	 */
	static inline NORETURN void processor_poweroff(void)
	{
		linux64_processor_poweroff();
	}

/**@endcond*/

#endif /* PROCESSOR_LINUX64_H_ */
