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

#ifndef ARCH_CORE_OR1K_CTX_H_
#define ARCH_CORE_OR1K_CTX_H_

	#ifndef __NEED_CORE_CONTEXT
		#error "do not include this file"
	#endif

/**
 * @addtogroup or1k-core-context Context
 * @ingroup or1k-core
 *
 * @brief Execution Context
 */
/**@{*/

	#include <arch/core/or1k/core.h>
	#include <nanvix/klib.h>

	/**
	 * @brief Execution context size (in bytes).
	 */
	#define OR1K_CONTEXT_SIZE 144

	/**
	 * @name Offsets to the Context Structure
	 */
	/**@{*/
	#define OR1K_CONTEXT_R0     0 /**< General Purpose Register  0     */
	#define OR1K_CONTEXT_R1     4 /**< General Purpose Register  1     */
	#define OR1K_CONTEXT_R2     8 /**< General Purpose Register  2     */
	#define OR1K_CONTEXT_R3    12 /**< General Purpose Register  3     */
	#define OR1K_CONTEXT_R4    16 /**< General Purpose Register  4     */
	#define OR1K_CONTEXT_R5    20 /**< General Purpose Register  5     */
	#define OR1K_CONTEXT_R6    24 /**< General Purpose Register  6     */
	#define OR1K_CONTEXT_R7    28 /**< General Purpose Register  7     */
	#define OR1K_CONTEXT_R8    32 /**< General Purpose Register  8     */
	#define OR1K_CONTEXT_R9    36 /**< General Purpose Register  9     */
	#define OR1K_CONTEXT_R10   40 /**< General Purpose Register 10     */
	#define OR1K_CONTEXT_R11   44 /**< General Purpose Register 11     */
	#define OR1K_CONTEXT_R12   48 /**< General Purpose Register 12     */
	#define OR1K_CONTEXT_R13   52 /**< General Purpose Register 13     */
	#define OR1K_CONTEXT_R14   56 /**< General Purpose Register 14     */
	#define OR1K_CONTEXT_R15   60 /**< General Purpose Register 15     */
	#define OR1K_CONTEXT_R16   64 /**< General Purpose Register 16     */
	#define OR1K_CONTEXT_R17   68 /**< General Purpose Register 17     */
	#define OR1K_CONTEXT_R18   72 /**< General Purpose Register 18     */
	#define OR1K_CONTEXT_R19   76 /**< General Purpose Register 19     */
	#define OR1K_CONTEXT_R20   80 /**< General Purpose Register 20     */
	#define OR1K_CONTEXT_R21   84 /**< General Purpose Register 21     */
	#define OR1K_CONTEXT_R22   88 /**< General Purpose Register 22     */
	#define OR1K_CONTEXT_R23   92 /**< General Purpose Register 23     */
	#define OR1K_CONTEXT_R24   96 /**< General Purpose Register 24     */
	#define OR1K_CONTEXT_R25  100 /**< General Purpose Register 25     */
	#define OR1K_CONTEXT_R26  104 /**< General Purpose Register 26     */
	#define OR1K_CONTEXT_R27  108 /**< General Purpose Register 27     */
	#define OR1K_CONTEXT_R28  112 /**< General Purpose Register 28     */
	#define OR1K_CONTEXT_R29  116 /**< General Purpose Register 29     */
	#define OR1K_CONTEXT_R30  120 /**< General Purpose Register 30     */
	#define OR1K_CONTEXT_R31  124 /**< General Purpose Register 31     */
	#define OR1K_CONTEXT_EPCR 128 /**< Shadow Program Counter Register */
	#define OR1K_CONTEXT_EEAR 132 /**< Shadow Effective Address        */
	#define OR1K_CONTEXT_ESR  136 /**< Shadow Status Register          */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @cond or1k
	 */

		/**
		 * Saved execution context.
		 */
		struct context
		{
			or1k_word_t  r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7; /**< General Purpose Registers  0 to  7 */
			or1k_word_t  r8,  r9, r10, r11, r12, r13, r14, r15; /**< General Purpose Registers  8 to 15 */
			or1k_word_t r16, r17, r18, r19, r20, r21, r22, r23; /**< General Purpose Registers 16 to 23 */
			or1k_word_t r24, r25, r26, r27, r28, r29, r30, r31; /**< General Purpose Registers 24 to 31 */
			or1k_word_t epcr;                                   /**< Shadow Program Counter Register    */
			or1k_word_t eear;                                   /**< Shadow Effective Address Register  */
			or1k_word_t  esr;                                   /**< Shadow Status Register             */
			or1k_byte_t RESERVED[4];                            /**< Required padding                   */
		} PACK;

	/**@endcond*/

	/**
	 * @brief Dumps context information.
	 *
	 * @param ctx Saved execution context.
	 */
	EXTERN void or1k_context_dump(const struct context *ctx);

	/**
	 * @brief Gets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the stack pointer register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline or1k_word_t or1k_context_get_sp(const struct context *ctx)
	{
		return (ctx->r1);
	}

	/**
	 * @brief Gets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the program conter register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline or1k_word_t or1k_context_get_pc(const struct context *ctx)
	{
		return (ctx->epcr);
	}

	/**
	 * @brief Sets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	static inline void or1k_context_set_sp(struct context *ctx, or1k_word_t val)
	{
		ctx->r1 = val;
	}

	/**
	 * @brief Sets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	static inline void or1k_context_set_pc(struct context *ctx, or1k_word_t val)
	{
		ctx->epcr = val;
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
 */

	/*
	 * Exported Constants
	 */
	/**@{*/
	#define CONTEXT_SIZE OR1K_CONTEXT_SIZE /**< @see OR1K_CONTEXT_SIZE */
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
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see or1k_context_get_sp().
	 */
	static inline word_t context_get_sp(const struct context *ctx)
	{
		return (or1k_context_get_sp(ctx));
	}

	/**
	 * @see or1k_context_get_pc().
	 */
	static inline word_t context_get_pc(const struct context *ctx)
	{
		return (or1k_context_get_pc(ctx));
	}

	/**
	 * @see or1k_context_set_sp().
	 */
	static inline void context_set_sp(struct context *ctx, word_t val)
	{
		or1k_context_set_sp(ctx, val);
	}

	/**
	 * @see or1k_context_set_pc().
	 */
	static inline void context_set_pc(struct context *ctx, word_t val)
	{
		or1k_context_set_pc(ctx, val);
	}

	/**
	 * @see or1k_context_dump().
	 */
	static inline void context_dump(const struct context *ctx)
	{
		or1k_context_dump(ctx);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_CTX_H_ */
