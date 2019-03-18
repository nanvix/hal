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

#ifndef _NANVIX_HAL_PROCESSOR_PROCESSOR_H_
#define _NANVIX_HAL_PROCESSOR_PROCESSOR_H_

	/**
	 * @defgroup processors Processors
	 */

	#if (defined(__k1b__))

		#undef  __NEED_PROCESSOR_BOSTAN
		#define __NEED_PROCESSOR_BOSTAN
		#include <arch/processor/bostan.h>

	#elif (defined(__i486__))

		#undef  __NEED_PROCESSOR_I486_QEMU
		#define __NEED_PROCESSOR_I486_QEMU
		#include <arch/processor/i486-qemu.h>

	#elif (defined(__optimsoc__))

		#undef  __NEED_PROCESSOR_OR1K_OPTIMSOC
		#define __NEED_PROCESSOR_OR1K_OPTIMSOC
		#include <arch/processor/optimsoc.h>

	#elif (defined(__or1k__))

		#undef  __NEED_PROCESSOR_OR1K_QEMU
		#define __NEED_PROCESSOR_OR1K_QEMU
		#include <arch/processor/or1k-qemu.h>

	#else

		#error "unkonwn processor"

	#endif

	#undef  __NEED_HAL_CLUSTER
	#define __NEED_HAL_CLUSTER
	#include <nanvix/hal/cluster.h>

#endif /* _NANVIX_HAL_PROCESSOR_PROCESSOR_H_ */
