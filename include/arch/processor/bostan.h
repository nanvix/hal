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

#ifndef PROCESSOR_BOSTAN_H_
#define PROCESSOR_BOSTAN_H_

	#ifndef __NEED_PROCESSOR_BOSTAN
		#error "bad processor configuration?"
	#endif

	/* Processor Interface Implementation */
	#include <arch/processor/bostan/_bostan.h>

/**
 * @addtogroup processor-bostan Bostan Processor
 * @ingroup processors
 *
 * @brief Bostan Processor
 */
/**@*/

	#include <arch/processor/bostan/clusters.h>
	#include <arch/processor/bostan/noc.h>

	/**
	 * @brief Powers off the underlying processor.
	 */
	EXTERN NORETURN void bostan_processor_poweroff(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond bostan
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
	 * @see bostan_processor_poweroff().
	 */
	static inline NORETURN void processor_poweroff(void)
	{
		bostan_processor_poweroff();
	}

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_H_ */
