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

#ifndef ARCH_CORE_ARM64_CTX_H_
#define ARCH_CORE_ARM64_CTX_H_

	#ifndef __NEED_CORE_CONTEXT
		#error "do not include this file"
	#endif

/**
 * @addtogroup arm64-core-context Context
 * @ingroup arm64-core
 *
 * @brief Execution Context Interface
 */
/**@{*/

#include <arch/core/arm64/core.h>
#include <nanvix/const.h>

#ifndef _ASM_FILE_

	/**
	 * @cond arm64
	 */

		/**
		 * Saved execution context upon interrupts and exceptions.
		 */
		struct context
		{
			arm64_word_t x0, x1, x2, x3, x4, x5, x6, x7;             /**< Function Argument/Return Value Registers   */
			arm64_word_t x8, x9, x10, x11, x12, x13, x14, x15;       /**< CorruptibleRegisters                       */
			arm64_word_t x16, x17, x18, x19, x20, x21, x22;          /**< Saved Registers                            */
			arm64_word_t x23, x24, x25, x26, x27, x28;               /**< Saved Registers                            */
			arm64_word_t x29;                                        /**< Frame Pointer Register                  */
			arm64_word_t x30;                                        /**< Return Address Register               */
			//arm64_word_t sp;                     /**< stack Pointer Register                   */
		} PACK ALIGN(ARM64_DWORD_SIZE);

	/**
	 * @brief Dumps context information.
	 *
	 * @param ctx Saved execution context.
	 */
	EXTERN void arm64_context_dump(const struct context *ctx);

	/**@endcond*/
#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
 */

	/*
	 * Exported Constants
	 */
	/**@{*/
	#define CONTEXT_SIZE 248 /**< @see ARM64_CONTEXT_SIZE */
	/**@}*/

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __context_struct /**< @see context */
	/**@}*/

	/**
	 * @brief Exported Functions
	 */
	/**@{*/
	#define __context_get_sp_fn /**< context_get_sp() */
	#define __context_get_pc_fn /**< context_get_pc() */
	#define __context_set_sp_fn /**< context_set_sp() */
	#define __context_set_pc_fn /**< context_set_pc() */
	#define __context_dump_fn   /**< context_dump()   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see arm64_context_get_sp().
	 */
	static inline arm64_word_t context_get_sp(const struct context *ctx)
	{
		UNUSED(ctx);
		return 0;
	}

	/**
	 * @see arm64_context_get_pc().
	 */
	static inline arm64_word_t context_get_pc(const struct context *ctx)
	{
		UNUSED(ctx);
		return 0;
	}

	/**
	 * @see arm64_context_set_sp().
	 */
	static inline void context_set_sp(struct context *ctx, arm64_word_t val)
	{
		UNUSED(ctx);
		UNUSED(val);
	}

	/**
	 * @see arm64_context_set_pc().
	 */
	static inline void context_set_pc(struct context *ctx, arm64_word_t val)
	{
		UNUSED(ctx);
		UNUSED(val);
	}

	/**
	 * @see arm64_context_dump().
	 */
	static inline void context_dump(const struct context *ctx)
	{
		arm64_context_dump(ctx);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_CTX_H_ */
