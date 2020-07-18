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

#ifndef ARCH_CORE_LINUX64_CTX_H_
#define ARCH_CORE_LINUX64_CTX_H_

	#ifndef __NEED_CORE_CONTEXT
		#error "do not include this file"
	#endif

/**
 * @addtogroup linux64-core-context Context
 * @ingroup linux64-core
 *
 * @brief Execution Context Interface
 */

	#include <arch/core/linux64/core.h>
	#include <nanvix/const.h>
	#include <nanvix/hlib.h>

	/**
	 * @brief Execution context size (in bytes).
	 */
	#define LINUX64_CONTEXT_SIZE 8

	/**@}*/

/**
 * @cond linux64
 */

	/**
	 * Saved execution context upon interrupts and exceptions.
	 */
	struct context
	{
		linux64_dword_t id;
	} PACK ALIGN(LINUX64_DWORD_SIZE);

/**@endcond*/

	/**
	 * @brief Dumps information about a saved execution context.
	 *
	 * @param ctx Saved exception context.
	 */
	EXTERN void linux64_ctx_dump(const struct context *ctx);

	/**
	 * @brief Get the value of the core.
	 *
	 * @param ctx Target context.
	 */
	static inline linux64_dword_t linux64_context_get_id(const struct context *ctx)
	{
		return (ctx->id);
	}

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/*
	 * Exported Constants
	 */
	/**@{*/
	#define CONTEXT_SIZE LINUX64_CONTEXT_SIZE /**< @see LINUX64_CONTEXT_SIZE */
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
	#define __context_get_id_fn /**< context_get_id() */
	#define __context_get_sp_fn /**< context_get_sp() */
	#define __context_get_pc_fn /**< context_get_pc() */
	#define __context_set_sp_fn /**< context_set_sp() */
	#define __context_set_pc_fn /**< context_set_pc() */
	#define __context_dump_fn   /**< context_dump()   */
	/**@}*/

	/**
	 * @brief Dummy function.
	 */
	static inline word_t context_get_sp(const struct context *ctx)
	{
		UNUSED(ctx);

		return (0);
	}

	/**
	 * @brief Dummy function.
	 */
	static inline word_t context_get_pc(const struct context *ctx)
	{
		UNUSED(ctx);

		return (0);
	}

	/**
	 * @brief Dummy function.
	 */
	static inline void context_set_sp(struct context *ctx, word_t val)
	{
		UNUSED(ctx);
		UNUSED(val);
	}

	/**
	 * @brief Dummy function.
	 */
	static inline void context_set_pc(struct context *ctx, word_t val)
	{
		UNUSED(ctx);
		UNUSED(val);
	}

	/**
	 * @brief Get the value of the core.
	 *
	 * @param ctx Target context.
	 */
	static inline dword_t context_get_id(const struct context *ctx)
	{
		return (linux64_context_get_id(ctx));
	}

	/**
	 * @see linux64_ctx_dump().
	 */
	static inline void context_dump(const struct context *ctx)
	{
		linux64_ctx_dump(ctx);
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_CTX_H_ */
