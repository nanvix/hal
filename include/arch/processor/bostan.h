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

	/**
	 * @brief Asynchronous IO Control Structure
	 */
	struct aiocb
	{
		int fd;            /**< File descriptor.           */
		int type;          /**< Type of operation (RX/TX). */
		spinlock_t * lock; /**< Operation lock.            */
	};

	#include <arch/processor/bostan/clusters.h>
	#include <arch/processor/bostan/noc.h>

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond bostan
 */

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __aiocb_struct /**< @see aiocb */
	/**@}*/

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define PROCESSOR_IS_MULTICLUSTER 1 /**< Multicluster feature */
	#define PROCESSOR_HAS_NOC         1 /**< NoC feature          */
	/**@}*/

/**@endcond*/

#endif /* PROCESSOR_BOSTAN_H_ */

