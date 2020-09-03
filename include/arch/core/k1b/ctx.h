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

#ifndef ARCH_CORE_K1B_CTX_H_
#define ARCH_CORE_K1B_CTX_H_

	#ifndef __NEED_CORE_CONTEXT
		#error "do not include this file"
	#endif

/**
 * @addtogroup k1b-core-context Context
 * @ingroup k1b-core
 *
 * @brief Execution Context Interface
 */
/**@{*/

#ifndef _ASM_FILE_
	#include <arch/core/k1b/core.h>
	#include <arch/core/k1b/mmu.h>
	#include <nanvix/const.h>
	#include <nanvix/hlib.h>
#endif

	/**
	 * @brief Execution context size (in bytes).
	 */
	#define K1B_CONTEXT_SIZE 320

	/**
	 * @name Offsets to the Context Structure
	 */
	/**@{*/
	#define K1B_CONTEXT_R0     0*K1B_WORD_SIZE /**< General Purpose Register  0                     */
	#define K1B_CONTEXT_R1     1*K1B_WORD_SIZE /**< General Purpose Register  1                     */
	#define K1B_CONTEXT_R2     2*K1B_WORD_SIZE /**< General Purpose Register  2                     */
	#define K1B_CONTEXT_R3     3*K1B_WORD_SIZE /**< General Purpose Register  3                     */
	#define K1B_CONTEXT_R4     4*K1B_WORD_SIZE /**< General Purpose Register  4                     */
	#define K1B_CONTEXT_R5     5*K1B_WORD_SIZE /**< General Purpose Register  5                     */
	#define K1B_CONTEXT_R6     6*K1B_WORD_SIZE /**< General Purpose Register  6                     */
	#define K1B_CONTEXT_R7     7*K1B_WORD_SIZE /**< General Purpose Register  7                     */
	#define K1B_CONTEXT_R8     8*K1B_WORD_SIZE /**< General Purpose Register  8                     */
	#define K1B_CONTEXT_R9     9*K1B_WORD_SIZE /**< General Purpose Register  9                     */
	#define K1B_CONTEXT_R10   10*K1B_WORD_SIZE /**< General Purpose Register 10                     */
	#define K1B_CONTEXT_R11   11*K1B_WORD_SIZE /**< General Purpose Register 11                     */
	#define K1B_CONTEXT_R12   12*K1B_WORD_SIZE /**< General Purpose Register 12                     */
	#define K1B_CONTEXT_R13   13*K1B_WORD_SIZE /**< General Purpose Register 13                     */
	#define K1B_CONTEXT_R14   14*K1B_WORD_SIZE /**< General Purpose Register 14                     */
	#define K1B_CONTEXT_R15   15*K1B_WORD_SIZE /**< General Purpose Register 15                     */
	#define K1B_CONTEXT_R16   16*K1B_WORD_SIZE /**< General Purpose Register 16                     */
	#define K1B_CONTEXT_R17   17*K1B_WORD_SIZE /**< General Purpose Register 17                     */
	#define K1B_CONTEXT_R18   18*K1B_WORD_SIZE /**< General Purpose Register 18                     */
	#define K1B_CONTEXT_R19   19*K1B_WORD_SIZE /**< General Purpose Register 19                     */
	#define K1B_CONTEXT_R20   20*K1B_WORD_SIZE /**< General Purpose Register 20                     */
	#define K1B_CONTEXT_R21   21*K1B_WORD_SIZE /**< General Purpose Register 21                     */
	#define K1B_CONTEXT_R22   22*K1B_WORD_SIZE /**< General Purpose Register 22                     */
	#define K1B_CONTEXT_R23   23*K1B_WORD_SIZE /**< General Purpose Register 23                     */
	#define K1B_CONTEXT_R24   24*K1B_WORD_SIZE /**< General Purpose Register 24                     */
	#define K1B_CONTEXT_R25   25*K1B_WORD_SIZE /**< General Purpose Register 25                     */
	#define K1B_CONTEXT_R26   26*K1B_WORD_SIZE /**< General Purpose Register 26                     */
	#define K1B_CONTEXT_R27   27*K1B_WORD_SIZE /**< General Purpose Register 27                     */
	#define K1B_CONTEXT_R28   28*K1B_WORD_SIZE /**< General Purpose Register 28                     */
	#define K1B_CONTEXT_R29   29*K1B_WORD_SIZE /**< General Purpose Register 29                     */
	#define K1B_CONTEXT_R30   30*K1B_WORD_SIZE /**< General Purpose Register 30                     */
	#define K1B_CONTEXT_R31   31*K1B_WORD_SIZE /**< General Purpose Register 31                     */
	#define K1B_CONTEXT_R32   32*K1B_WORD_SIZE /**< General Purpose Register 32                     */
	#define K1B_CONTEXT_R33   33*K1B_WORD_SIZE /**< General Purpose Register 33                     */
	#define K1B_CONTEXT_R34   34*K1B_WORD_SIZE /**< General Purpose Register 34                     */
	#define K1B_CONTEXT_R35   35*K1B_WORD_SIZE /**< General Purpose Register 35                     */
	#define K1B_CONTEXT_R36   36*K1B_WORD_SIZE /**< General Purpose Register 36                     */
	#define K1B_CONTEXT_R37   37*K1B_WORD_SIZE /**< General Purpose Register 37                     */
	#define K1B_CONTEXT_R38   38*K1B_WORD_SIZE /**< General Purpose Register 38                     */
	#define K1B_CONTEXT_R39   39*K1B_WORD_SIZE /**< General Purpose Register 39                     */
	#define K1B_CONTEXT_R40   40*K1B_WORD_SIZE /**< General Purpose Register 40                     */
	#define K1B_CONTEXT_R41   41*K1B_WORD_SIZE /**< General Purpose Register 41                     */
	#define K1B_CONTEXT_R42   42*K1B_WORD_SIZE /**< General Purpose Register 42                     */
	#define K1B_CONTEXT_R43   43*K1B_WORD_SIZE /**< General Purpose Register 43                     */
	#define K1B_CONTEXT_R44   44*K1B_WORD_SIZE /**< General Purpose Register 44                     */
	#define K1B_CONTEXT_R45   45*K1B_WORD_SIZE /**< General Purpose Register 45                     */
	#define K1B_CONTEXT_R46   46*K1B_WORD_SIZE /**< General Purpose Register 46                     */
	#define K1B_CONTEXT_R47   47*K1B_WORD_SIZE /**< General Purpose Register 47                     */
	#define K1B_CONTEXT_R48   48*K1B_WORD_SIZE /**< General Purpose Register 48                     */
	#define K1B_CONTEXT_R49   49*K1B_WORD_SIZE /**< General Purpose Register 49                     */
	#define K1B_CONTEXT_R50   50*K1B_WORD_SIZE /**< General Purpose Register 50                     */
	#define K1B_CONTEXT_R51   51*K1B_WORD_SIZE /**< General Purpose Register 51                     */
	#define K1B_CONTEXT_R52   52*K1B_WORD_SIZE /**< General Purpose Register 52                     */
	#define K1B_CONTEXT_R53   53*K1B_WORD_SIZE /**< General Purpose Register 53                     */
	#define K1B_CONTEXT_R54   54*K1B_WORD_SIZE /**< General Purpose Register 54                     */
	#define K1B_CONTEXT_R55   55*K1B_WORD_SIZE /**< General Purpose Register 55                     */
	#define K1B_CONTEXT_R56   56*K1B_WORD_SIZE /**< General Purpose Register 56                     */
	#define K1B_CONTEXT_R57   57*K1B_WORD_SIZE /**< General Purpose Register 57                     */
	#define K1B_CONTEXT_R58   58*K1B_WORD_SIZE /**< General Purpose Register 58                     */
	#define K1B_CONTEXT_R59   59*K1B_WORD_SIZE /**< General Purpose Register 59                     */
	#define K1B_CONTEXT_R60   60*K1B_WORD_SIZE /**< General Purpose Register 60                     */
	#define K1B_CONTEXT_R61   61*K1B_WORD_SIZE /**< General Purpose Register 61                     */
	#define K1B_CONTEXT_R62   62*K1B_WORD_SIZE /**< General Purpose Register 62                     */
	#define K1B_CONTEXT_R63   63*K1B_WORD_SIZE /**< General Purpose Register 63                     */
	#define K1B_CONTEXT_PS    64*K1B_WORD_SIZE /**< Processing Status Register                      */
	#define K1B_CONTEXT_SPS   65*K1B_WORD_SIZE /**< Shadow Processing Status Register               */
	#define K1B_CONTEXT_SSPS  66*K1B_WORD_SIZE /**< Shadow Shadow Processing Status Register        */
	#define K1B_CONTEXT_SSSPS 67*K1B_WORD_SIZE /**< Shadow Shadow Shadow Processing Status Register */
	#define K1B_CONTEXT_SPC   68*K1B_WORD_SIZE /**< Shadow Program Counter Register                 */
	#define K1B_CONTEXT_SSPC  69*K1B_WORD_SIZE /**< Shadow Shadow Program Counter Register          */
	#define K1B_CONTEXT_SSSPC 70*K1B_WORD_SIZE /**< Shadow Shadow Shadow Program Counter Register   */
	#define K1B_CONTEXT_SSP   71*K1B_WORD_SIZE /**< Shadow Stack Pointer Register                   */
	#define K1B_CONTEXT_SSSP  72*K1B_WORD_SIZE /**< Shadow Shadow Stack Pointer Register            */
	#define K1B_CONTEXT_SSSSP 73*K1B_WORD_SIZE /**< Shadow Shadow Shadown Stack Pointer Register    */
	#define K1B_CONTEXT_KSP   74*K1B_WORD_SIZE /**< Kernel Stack Pointer Register                   */
	#define K1B_CONTEXT_RA    75*K1B_WORD_SIZE /**< Return Address Register                         */
	#define K1B_CONTEXT_CS    76*K1B_WORD_SIZE /**< Compute Status Register                         */
	#define K1B_CONTEXT_LC    77*K1B_WORD_SIZE /**< Loop Count Register                             */
	#define K1B_CONTEXT_LS    78*K1B_WORD_SIZE /**< Loop Start Register                             */
	#define K1B_CONTEXT_LE    79*K1B_WORD_SIZE /**< Loop Exit Register                              */
	/**@}*/

	/**
	 * @brief Execution context size (in bytes).
	 */
	#define K1B_CONTEXT_MAGIC_NUMBER 0xdeadbeef;

#ifndef _ASM_FILE_

	/**
	 * @cond k1b
	 */

		/**
		 * Saved execution context upon interrupts and exceptions.
		 */
		struct context
		{
			k1b_word_t  r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7; /**< General Purpose Registers  0 to  7 */
			k1b_word_t  r8,  r9, r10, r11, r12, r13, r14, r15; /**< General Purpose Registers  8 to 15 */
			k1b_word_t r16, r17, r18, r19, r20, r21, r22, r23; /**< General Purpose Registers 16 to 23 */
			k1b_word_t r24, r25, r26, r27, r28, r29, r30, r31; /**< General Purpose Registers 24 to 31 */
			k1b_word_t r32, r33, r34, r35, r36, r37, r38, r39; /**< General Purpose Registers 32 to 39 */
			k1b_word_t r40, r41, r42, r43, r44, r45, r46, r47; /**< General Purpose Registers 40 to 47 */
			k1b_word_t r48, r49, r50, r51, r52, r53, r54, r55; /**< General Purpose Registers 48 to 55 */
			k1b_word_t r56, r57, r58, r59, r60, r61, r62, r63; /**< General Purpose Registers 56 to 63 */
			k1b_word_t ps, sps, ssps, sssps;                   /**< Processing Status Registers        */
			k1b_word_t spc, sspc, ssspc;                       /**< Shadow Program Counter Registers   */
			k1b_word_t ssp, sssp, ssssp, ksp;                  /**< Shadow Stack Pointer Registers     */
			k1b_word_t ra;                                     /**< Return Address Register            */
			k1b_word_t cs;                                     /**< Compute Status Register            */
			k1b_word_t lc;                                     /**< Loop Count Register                */
			k1b_word_t ls;                                     /**< Loop Start Register                */
			k1b_word_t le;                                     /**< Loop Exit Register                 */
		} PACK ALIGN(K1B_WORD_SIZE);

		/**
		 * Execution stack.
		 */
		struct stack
		{
			k1b_word_t magic;                                     /**< Magic number to protection.                   */
			k1b_word_t data[(K1B_PAGE_SIZE / K1B_WORD_SIZE) - 2]; /**< Data.                                         */
			k1b_word_t begin;                                     /**< First address because stack grows negatively. */
		} PACK ALIGN(K1B_PAGE_SIZE);

	/**@endcond*/

	/**
	 * @brief Dumps context information.
	 *
	 * @param ctx Saved execution context.
	 */
	EXTERN void k1b_context_dump(const struct context *ctx);

	/**
	 * @brief Create a context.
	 *
	 * @param start  Start routine.
	 * @param ustack User stack pointer.
	 * @param kstack Kernel stack pointer.
	 *
	 * @returns Valid pointer if the context struct was created
	 * into the kernel stack correctly, NULL pointer otherwise.
	 */
	EXTERN struct context * k1b_context_create(
		void (* start)(void),
		struct stack * ustack,
		struct stack * kstack
	);

	/**
	 * @brief Gets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the stack pointer register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline k1b_word_t k1b_context_get_sp(const struct context *ctx)
	{
		return (ctx->ssp);
	}

	/**
	 * @brief Gets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 *
	 * @returns The value of the program conter register, which is
	 * saved in the saved execution context pointed to by @p ctx.
	 */
	static inline k1b_word_t k1b_context_get_pc(const struct context *ctx)
	{
		return (ctx->spc);
	}

	/**
	 * @brief Sets the value of the stack pointer register.
	 *
	 * @param ctx Target context.
	 * @param  val Value to store.
	 */
	static inline void k1b_context_set_sp(struct context *ctx, k1b_word_t val)
	{
		ctx->ssp = val;
	}

	/**
	 * @brief Sets the value of the program conter register.
	 *
	 * @param ctx Target context.
	 * @param val Value to store.
	 */
	static inline void k1b_context_set_pc(struct context *ctx, k1b_word_t val)
	{
		ctx->spc = val;
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/*
	 * Exported Constants
	 */
	/**@{*/
	#define CONTEXT_SIZE K1B_CONTEXT_SIZE                 /**< @see K1B_CONTEXT_SIZE         */
	#define CONTEXT_MAGIC_NUMBER K1B_CONTEXT_MAGIC_NUMBER /**< @see K1B_CONTEXT_MAGIC_NUMBER */
	/**@}*/

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __context_struct /**< @see context */
	#define __stack_struct   /**< @see stack   */
	/**@}*/

	/**
	 * @brief Exported Functions
	 */
	/**@{*/
	#define __context_get_sp_fn    /**< context_get_sp()    */
	#define __context_get_pc_fn    /**< context_get_pc()    */
	#define __context_set_sp_fn    /**< context_set_sp()    */
	#define __context_set_pc_fn    /**< context_set_pc()    */
	#define __context_dump_fn      /**< context_dump()      */
	#define __context_create_fn    /**< context_create()    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see k1b_context_get_sp().
	 */
	static inline word_t context_get_sp(const struct context *ctx)
	{
		return (k1b_context_get_sp(ctx));
	}

	/**
	 * @see k1b_context_get_pc().
	 */
	static inline word_t context_get_pc(const struct context *ctx)
	{
		return (k1b_context_get_pc(ctx));
	}

	/**
	 * @see k1b_context_set_sp().
	 */
	static inline void context_set_sp(struct context *ctx, word_t val)
	{
		k1b_context_set_sp(ctx, val);
	}

	/**
	 * @see k1b_context_set_pc().
	 */
	static inline void context_set_pc(struct context *ctx, word_t val)
	{
		k1b_context_set_pc(ctx, val);
	}

	/**
	 * @see k1b_context_dump().
	 */
	static inline void context_dump(const struct context *ctx)
	{
		k1b_context_dump(ctx);
	}

	/**
	 * @see k1b_context_create()
	 */
	#define __context_create(start, ustack, kstack) \
		k1b_context_create(start, ustack, kstack)

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_K1B_CTX_H_ */
