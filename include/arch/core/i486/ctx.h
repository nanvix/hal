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

#ifndef ARCH_CORE_I486_CTX_H_
#define ARCH_CORE_I486_CTX_H_

	#ifndef __NEED_CORE_CONTEXT
		#error "do not include this file"
	#endif

/**
 * @addtogroup i486-core-context Context
 * @ingroup i486-core
 *
 * @brief Execution Context Interface
 */

	#include <arch/core/i486/core.h>

	/**
	 * @brief Hardware-saved execution context size (in bytes).
	 */
	#define I486_CONTEXT_HW_SIZE 20

	/**
	 * @brief Software-saved execution context size (in bytes).
	 */
	#define I486_CONTEXT_SW_SIZE 44

	/**
	 * @brief Execution context size (in bytes).
	 */
	#define I486_CONTEXT_SIZE (I486_CONTEXT_HW_SIZE + I486_CONTEXT_SW_SIZE)

	/**
	 * @name Offsets to the Context Structure
	 */
	/**@{*/
	#define I486_CONTEXT_GS       0 /**< Extra Data Segment #3 Register        */
	#define I486_CONTEXT_FS       4 /**< Extra Data Segment #2 Register        */
	#define I486_CONTEXT_ES       8 /**< Extra Data Segment #1 Register        */
	#define I486_CONTEXT_DS      12 /**< Data Segment Register                 */
	#define I486_CONTEXT_EDI     16 /**< Extended Destination Index Register   */
	#define I486_CONTEXT_ESI     20 /**< Extended Source Index Register        */
	#define I486_CONTEXT_EBP     24 /**< Extended Stack base Pointer Register  */
	#define I486_CONTEXT_EDX     28 /**< Extended Accumulator #2 Register      */
	#define I486_CONTEXT_ECX     32 /**< Extended Counter Register             */
	#define I486_CONTEXT_EBX     36 /**< Extended Base Index Register          */
	#define I486_CONTEXT_EAX     40 /**< Exntended Accumulator #1 Register     */
	#define I486_CONTEXT_EIP     44 /**< Extended Instruction Pointer Register */
	#define I486_CONTEXT_CS      48 /**< Code Segment Register                 */
	#define I486_CONTEXT_EFLAGS  52 /**< Exended Flags Register                */
	#define I486_CONTEXT_ESP     56 /**< Extended Stack Pointer Register       */
	#define I486_CONTEXT_SS      60 /**< Stack Segment Register                */
	/**@}*/

#ifndef _ASM_FILE_

/**
 * @cond i486
 */

	/**
	 * Saved execution context upon interrupts and exceptions.
	 */
	struct context
	{
        i486_word_t gs, fs, es, ds;                    /**< Segment Registers         */
        i486_word_t edi, esi, ebp, edx, ecx, ebx, eax; /**< General Purpose Registers */
        i486_word_t eip, cs, eflags, useresp, ss;      /**< Special Registers         */
	} PACK ALIGN(I486_WORD_SIZE);

/**@endcond*/

	/**
	 * @brief Dumps information about a saved execution context.
	 *
	 * @param ctx Saved exception context.
	 */
	EXTERN void i486_ctx_dump(const struct context *ctx);

	/**
	 * @brief Gets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the stack pointer register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline i486_word_t i486_context_get_sp(const struct context *ctx)
	{
		return (ctx->useresp);
	}

	/**
	 * @brief Gets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the program conter register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline i486_word_t i486_context_get_pc(const struct context *ctx)
	{
		return (ctx->eip);
	}

	/**
	 * @brief Sets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	static inline void i486_context_set_sp(struct context *ctx, i486_word_t val)
	{
		ctx->useresp = val;
	}

	/**
	 * @brief Sets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	static inline void i486_context_set_pc(struct context *ctx, i486_word_t val)
	{
		ctx->eip = val;
	}

#endif /* _ASM_FILE_ */

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/*
	 * Exported Constants
	 */
	/**@{*/
	#define CONTEXT_SIZE I486_CONTEXT_SIZE /**< @see I486_CONTEXT_SIZE */
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
	 * @see i486_context_get_sp().
	 */
	static inline word_t context_get_sp(const struct context *ctx)
	{
		return (i486_context_get_sp(ctx));
	}

	/**
	 * @see i486_context_get_pc().
	 */
	static inline word_t context_get_pc(const struct context *ctx)
	{
		return (i486_context_get_pc(ctx));
	}

	/**
	 * @see i486_context_set_sp().
	 */
	static inline void context_set_sp(struct context *ctx, word_t val)
	{
		i486_context_set_sp(ctx, val);
	}

	/**
	 * @see i486_context_set_pc().
	 */
	static inline void context_set_pc(struct context *ctx, word_t val)
	{
		i486_context_set_pc(ctx, val);
	}

	/**
	 * @see i486_ctx_dump().
	 */
	static inline void context_dump(const struct context *ctx)
	{
		i486_ctx_dump(ctx);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_CTX_H_ */
