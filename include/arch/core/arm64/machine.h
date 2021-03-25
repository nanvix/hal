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


#ifndef ARCH_CORE_ARM64_MACHINE_H_
#define ARCH_CORE_ARM64_MACHINE_H_

/**
 * @addtogroup arm64-core-machine Machine
 * @ingroup arm64-core
 *
 * @brief Machine Interface
 */
/**@{*/

	#ifndef __NEED_CORE_MACHINE
		#error "do not include this file"
	#endif

	/* Must come first. */
	#define __NEED_CORE_CONTEXT
	#define __NEED_CORE_TYPES

	#include <arch/core/arm64/ctx.h>
	#include <arch/core/arm64/types.h>
	#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Dumps all CSRs.
	 */
	EXTERN void arm64_dump_all_csr(void);

	/**
	 * @brief Handles a bad machine exception.
	 */
	EXTERN NORETURN void arm64_do_mbad(const struct context *ctx);

	/**
	 * @brief Handles machine calls.
	 *
	 * @param ctx Interrupted context
	 */
	EXTERN void arm64_do_mcall(struct context *ctx);

	/**
	 * @brief Handles machine exceptions.
	 *
	 * @param ctx Interrupted context
	 */
	EXTERN NORETURN void arm64_do_mexcp(const struct context *ctx);

	/**
	 * @brief Handles machine interrupts.
	 *
	 * @param ctx Interrupted context.
	 */
	EXTERN void arm64_do_mint(const struct context *ctx);

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_ARM64_MACHINE_H_ */
