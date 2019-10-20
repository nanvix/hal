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

#ifndef ARCH_CORE_OR1K_SPINLOCK_H_
#define ARCH_CORE_OR1K_SPINLOCK_H_

/**
 * @addtogroup or1k-core-spinlock Spinlock
 * @ingroup or1k-core
 *
 * @brief or1k Spinlocks
 */
/**@{*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <posix/stdint.h>

#endif /* _ASM_FILE_ */

	/**
	 * @name Spinlock State
	 */
	/**@{*/
	#define OR1K_SPINLOCK_UNLOCKED 0x0 /**< Unlocked */
	#define OR1K_SPINLOCK_LOCKED   0x1 /**< Locked   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Spinlock.
	 */
	typedef uint32_t or1k_spinlock_t;

	/**
	 * @brief Initializes a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void or1k_spinlock_init(or1k_spinlock_t *lock)
	{
		*lock = OR1K_SPINLOCK_UNLOCKED;
		__sync_synchronize();
	}

	/**
	 * @brief Attempts to lock a spinlock.
	 *
	 * @param lock Target spinlock.
	 *
	 * @returns Upon successful completion, the spinlock pointed to by
	 * @p lock is locked and zero is returned. Upon failure, non-zero
	 * is returned instead, and the lock is not acquired by the
	 * caller.
	 */
	static inline int or1k_spinlock_trylock(or1k_spinlock_t *lock)
	{
		return (
			!__sync_bool_compare_and_swap(
				lock,
				OR1K_SPINLOCK_UNLOCKED,
				OR1K_SPINLOCK_LOCKED
			)
		);
	}

	/**
	 * @brief Locks a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void or1k_spinlock_lock(or1k_spinlock_t *lock)
	{
		while (or1k_spinlock_trylock(lock))
			/* noop */;

		__sync_synchronize();
	}

	/**
	 * @brief Unlocks a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void or1k_spinlock_unlock(or1k_spinlock_t *lock)
	{
		*lock = OR1K_SPINLOCK_UNLOCKED;
		__sync_synchronize();
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond or1k
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
	#define SPINLOCK_UNLOCKED OR1K_SPINLOCK_UNLOCKED /**< @see OR1K_SPINLOCK_UNLOCKED */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see or1k_spinlock_t
	 */
	typedef or1k_spinlock_t spinlock_t;

	/**
	 * @see ork1_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		or1k_spinlock_init(lock);
	}

	/**
	 * @see ork1_spinlock_trylock().
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		return (or1k_spinlock_trylock(lock));
	}

	/**
	 * @see ork1_spinlock_lock().
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		or1k_spinlock_lock(lock);
	}

	/**
	 * @see ork1_spinlock_unlock().
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		or1k_spinlock_unlock(lock);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_OR1K_SPINLOCK_H_ */
