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

#ifndef PROCESSOR_RISCV32_H_
#define PROCESSOR_RISCV32_H_

	#ifndef __NEED_PROCESSOR_RISCV32
		#error "bad processor configuration?"
	#endif

	/* Processor Interface Implementation */
	#include <arch/processor/riscv32/_riscv32.h>

/**
 * @addtogroup processor-riscv32 RISCV-32
 * @ingroup processors
 *
 * @brief RISCV-32 Processor
 */
/**@*/

/**@}*/

/*============================================================================*
 * Provided Interface                                                         *
 *============================================================================*/
/**
 * @cond or1kqemu
 */

	/**
	 * @name Provided Features
	 */
	/**@{*/
	#define PROCESSOR_IS_MULTICLUSTER 0 /**< Multicluster feature */
	#define PROCESSOR_HAS_NOC         0 /**< NoC feature          */
	/**@}*/

/**@endcond*/

#endif /* PROCESSOR_RISCV32_H_ */
