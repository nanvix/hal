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

#ifndef ARCH_CORE_K1B_CORE_H_
#define ARCH_CORE_K1B_CORE_H_

/**
 * @addtogroup k1b-core
 */
/**@{*/

	/* Must comme first. */
	#define __NEED_CORE_TYPES

	#include <arch/core/k1b/types.h>
	#include <arch/core/k1b/spinlock.h>
	#include <HAL/hal/board/boot_args.h>
	#include <mOS_vcore_u.h>
	#include <nanvix/const.h>

	/**
	 * @brief Event line used for signals.
	 */
	#define K1B_EVENT_LINE 0

	/**
	 * @brief Resets the underlying core.
	 *
	 * The k1b_core_reset() function resets execution instruction in
	 * the underlying core by reseting the kernel stack to its initial
	 * location and relaunching the k1b_slave_setup() function.
	 *
	 * @note This function does not return.
	 * @note For the implementation of this function check out
	 * assembly source files.
	 *
	 * @see k1b_slave_setup()
	 *
	 * @author Pedro Henrique Penna
	 */
	EXTERN NORETURN void _k1b_core_reset(void);

	/**
	 * @brief Initializes the underlying core.
	 */
	EXTERN void k1b_core_setup(void);

	/**
	 * @brief Gets the ID of the core.
	 *
	 * The k1b_core_get_id() returns the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core.
	 */
	static inline int k1b_core_get_id(void)
	{
		return (__k1_get_cpu_id());
	}

	/**
	 * @brief Clears IPIs in the underlying core.
	 */
	static inline void k1b_core_clear(void)
	{
		mOS_pe_event_clear(K1B_EVENT_LINE);
	}

	/**
	 * @brief Waits and clears IPIs in the underlying core.
	 */
	static inline void k1b_core_waitclear(void)
	{
		mOS_pe_event_waitclear(K1B_EVENT_LINE);
	}

	/**
	 * @brief Sends a signal.
	 *
	 * The k1b_core_notify() function sends a signal to the core whose ID
	 * equals to @p coreid.
	 *
	 * @param coreid ID of the target core.
	 *
	 * @bug No sanity check is performed in @p coreid.
	 *
	 * @author Pedro Henrique Penna
	 */
	static inline void k1b_core_notify(int coreid)
	{
		mOS_pe_notify(
			1 << coreid,    /* Target cores.                            */
			K1B_EVENT_LINE, /* Event line.                              */
			1,              /* Notify an event? (I/O clusters only)     */
			0               /* Notify an interrupt? (I/O clusters only) */
		);
	}

	/**
	 * @brief Powers off the underlying core.
	 */
	static inline void k1b_core_poweroff(void)
	{
		mOS_exit(__k1_spawn_type() != __MPPA_MPPA_SPAWN, 0);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond k1b
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT       K1B_BYTE_BIT       /**< @see BYTE_BIT       */
	#define HWORD_BIT      K1B_HWORD_BIT      /**< @see HWORD_BIT      */
	#define WORD_BIT       K1B_WORD_BIT       /**< @see WORD_BIT       */
	#define DWORD_BIT      K1B_DWORD_BIT      /**< @see DWORD_BIT      */
	#define BYTE_SIZE      K1B_SIZE_SIZE      /**< @see BYTE_SIZE      */
	#define HWORD_SIZE     K1B_HWORD_SIZE     /**< @see HWORD_SIZE     */
	#define WORD_SIZE      K1B_WORD_SIZE      /**< @see WORD_SIZE      */
	#define DWORD_SIZE     K1B_DWORD_SIZE     /**< @see DWORD_SIZE     */
	/**@}*/

	/**
	 * @brief Exported Types
	 */
	/**@{*/
	#define __byte_t  /**< @see byte_t  */
	#define __hword_t /**< @see hword_t */
	#define __word_t  /**< @see word_t  */
	#define __dword_t /**< @see dword_t */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define ___core_reset_fn    /**< _core_reset()    */
	#define __core_clear_fn     /**< core_clear()     */
	#define __core_get_id_fn    /**< core_get_id()    */
	#define __core_notify_fn    /**< core_notify()    */
	#define __core_poweroff_fn  /**< core_poweroff()  */
	#define __core_setup_fn     /**< core_setup()     */
	#define __core_waitclear_fn /**< core_waitclear() */
	/**@}*/

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef k1b_byte_t  byte_t;  /**< Byte        */
	typedef k1b_hword_t hword_t; /**< Half Word   */
	typedef k1b_word_t  word_t;  /**< Word        */
	typedef k1b_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see _k1b_core_reset().
	 */
	static inline void _core_reset(void)
	{
		_k1b_core_reset();
	}

	/**
	 * @see k1b_core_clear().
	 */
	static inline void core_clear(void)
	{
		k1b_core_clear();
	}

	/**
	 * @see k1b_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (k1b_core_get_id());
	}

	/**
	 * @see k1b_core_notify()
	 */
	static inline void core_notify(int coreid)
	{
		k1b_core_notify(coreid);
	}

	/**
	 * @see k1b_core_poweroff().
	 */
	static inline void core_poweroff(void)
	{
		k1b_core_poweroff();
	}

	/**
	 * @see k1b_core_setup()
	 */
	static inline void core_setup()
	{
		k1b_core_setup();
	}

	/**
	 * @see k1b_core_waitclear().
	 */
	static inline void core_waitclear(void)
	{
		k1b_core_waitclear();
	}

/**@endcond*/

#endif /* ARCH_CORE_K1B_CORE_H_ */
