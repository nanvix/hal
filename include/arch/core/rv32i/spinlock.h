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

#ifndef ARCH_CORE_RV32I_SPINLOCK_H_
#define ARCH_CORE_RV32I_SPINLOCK_H_

/**
 * @addtogroup rv32i-core-spinlock Spinlock
 * @ingroup rv32i-core
 *
 * @brief rv32i Spinlocks
 */
/**@{*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <stdint.h>

#endif /* _ASM_FILE_ */

	/**
	 * @name Spinlock State
	 */
	/**@{*/
	#define RV32I_SPINLOCK_UNLOCKED 0x0 /**< Unlocked */
	#define RV32I_SPINLOCK_LOCKED   0x1 /**< Locked   */
	/**@}*/

	/**
	 * @brief Spinlock.
	 */
	typedef uint32_t rv32i_spinlock_t;

	/**
	 * @brief Initializes a rv32i_spinlock_t.
	 *
	 * @param lock Target rv32i_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32i_spinlock_init(rv32i_spinlock_t *lock)
	{
		*lock = RV32I_SPINLOCK_UNLOCKED;
	}

	/**
	 * @brief Attempts to lock a rv32i_spinlock_t.
	 *
	 * @param lock Target rv32i_spinlock_t.
	 *
	 * @returns Upon successful completion, the rv32i_spinlock_t pointed to by
	 * @p lock is locked and zero is returned. Upon failure, non-zero
	 * is returned instead, and the lock is not acquired by the
	 * caller.
	 *
	 * @todo Implement this function.
	 */
	static inline int rv32i_spinlock_trylock(rv32i_spinlock_t *lock)
	{
		return (
			!__sync_bool_compare_and_swap(
				lock,
				RV32I_SPINLOCK_UNLOCKED,
				RV32I_SPINLOCK_LOCKED
			)
		);
	}

	/**
	 * @brief Locks a rv32i_spinlock_t.
	 *
	 * @param lock Target rv32i_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32i_spinlock_lock(rv32i_spinlock_t *lock)
	{
		while (rv32i_spinlock_trylock(lock))
			/* noop */;
		__sync_synchronize();
	}

	/**
	 * @brief Unlocks a rv32i_spinlock_t.
	 *
	 * @param lock Target rv32i_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32i_spinlock_unlock(rv32i_spinlock_t *lock)
	{
		__sync_synchronize();
		*lock = RV32I_SPINLOCK_UNLOCKED;
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32i
 */

	/**
	 * @name Provided Interface
	 */
	/**@{*/
	#define __spinlock_t          /**< @see spinlock_t    */
	#define __spinlock_init_fn    /**< spinlock_init()    */
	#define __spinlock_lock_fn    /**< spinlock_lock()    */
	#define __spinlock_trylock_fn /**< spinlock_trylock() */
	#define __spinlock_unlock_fn  /**< spinlock_unlock()  */
	/**@}*/

	/**
	 * @name Spinlock State
	 */
	/**@{*/
	#define SPINLOCK_UNLOCKED RV32I_SPINLOCK_UNLOCKED /**< @see RV32I_SPINLOCK_UNLOCKED */
	#define SPINLOCK_LOCKED   RV32I_SPINLOCK_LOCKED   /**< @see RV32I_SPINLOCK_LOCKED   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see rv32i_spinlock_t
	 */
	typedef rv32i_spinlock_t spinlock_t;

	/**
	 * @see rv32i_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		rv32i_spinlock_init(lock);
	}

	/**
	 * @see rv32i_spinlock_trylock().
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		return (rv32i_spinlock_trylock(lock));
	}

	/**
	 * @see rv32i_spinlock_lock().
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		rv32i_spinlock_lock(lock);
	}

	/**
	 * @see rv32i_spinlock_unlock().
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		rv32i_spinlock_unlock(lock);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_RV32I_SPINLOCK_H_ */
