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

#ifndef _NANVIX_HAL_TARGET_TARGET_H_
#define _NANVIX_HAL_TARGET_TARGET_H_

	/**
	 * @defgroup targets Targets
	 */

	#if defined(__mppa256__)

		#undef  __NEED_TARGET_MPPA256
		#define __NEED_TARGET_MPPA256
		#include <arch/target/kalray/mppa256.h>

	#elif (defined(__optimsoc__))

		#undef  __NEED_TARGET_OPTIMSOC
		#define __NEED_TARGET_OPTIMSOC
		#include <arch/target/optimsoc/optimsoc.h>

	#elif (defined(__qemu_x86__))

		#undef  __NEED_TARGET_QEMU_I486_PC
		#define __NEED_TARGET_QEMU_I486_PC
		#include <arch/target/qemu/i486-pc.h>

	#elif (defined(__qemu_openrisc__))

		#undef  __NEED_TARGET_QEMU_OR1K_PC
		#define __NEED_TARGET_QEMU_OR1K_PC
		#include <arch/target/qemu/or1k-pc.h>

	#elif (defined(__qemu_riscv32__))

		#undef  __NEED_TARGET_QEMU_RISCV32
		#define __NEED_TARGET_QEMU_RISCV32
		#include <arch/target/qemu/riscv32.h>

	#else

		#error "unknown target"

	#endif

	#undef  __NEED_HAL_PROCESSOR
	#define __NEED_HAL_PROCESSOR
	#include <nanvix/hal/processor.h>

#endif /* _NANVIX_HAL_TARGET_TARGET_H_ */
