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

#ifndef ARCH_CORE_ARM64_CORE_H_
#define ARCH_CORE_ARM64_CORE_H_

	/* Must come first. */
	#define __NEED_CORE_TYPES
	#define __NEED_CORE_REGS

/**
 * @addtogroup arm64-core
 */
/**@{*/

	#include <arch/core/arm64-core/regs.h>
	#include <arch/core/arm64-core/types.h>
	#include <arch/core/arm64-core/mcall.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @see Gets the ID of the underlying core.
	 */
	static inline int arm64_core_get_id(void)
	{
		return (arm64_mcall_cpu_id());
	}

#endif /* _ASM_FILE_ */

#endif /* ARCH_CORE_ARM64_CORE_H_ */