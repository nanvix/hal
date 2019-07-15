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

#ifndef ARCH_CORE_LINUX64_SPINLOCK_H_
#define ARCH_CORE_LINUX64_SPINLOCK_H_

/**
 * @addtogroup linux64-core-spinlock Spinlock
 * @ingroup linux64-core
 *
 * @brief linux64 Spinlocks
 */
/**@{*/

	#include <pthread.h>

	/**
	 * @name Spinlock State
	 *
	 * @bug: We should wrap these.
	 */
	/**@{*/
	#define LINUX64_SPINLOCK_UNLOCKED PTHREAD_MUTEX_INITIALIZER /**< Unlocked */
	/**@}*/

	/**
	 * @brief Spinlock.
	 */
	typedef pthread_mutex_t linux64_spinlock_t;

	/**
	 * @brief Initializes a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void linux64_spinlock_init(linux64_spinlock_t *lock)
	{
		pthread_mutex_init(lock, NULL);
	}

	/**
	 * @brief Locks a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void linux64_spinlock_lock(linux64_spinlock_t *lock)
	{
		pthread_mutex_lock(lock);
	}

	/**
	 * @brief Attempts to lock a spinlock.
	 *
	 * @param lock Target spinlock.
	 *
	 * @returns Upon successful completion, the spinlock pointed to by
	 * @p lock is locked and non-zero is returned. Upon failure, zero
	 * is returned instead, and the lock is not acquired by the
	 * caller.
	 */
	static inline int linux64_spinlock_trylock(linux64_spinlock_t *lock)
	{
		return (!pthread_mutex_trylock(lock));
	}

	/**
	 * @brief Unlocks a spinlock.
	 *
	 * @param lock Target spinlock.
	 */
	static inline void linux64_spinlock_unlock(linux64_spinlock_t *lock)
	{
		pthread_mutex_unlock(lock);
	}

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/**
	 * @name Exported Types
	 */
	/**@{*/
	#define __spinlock_t 		  /**< @see spinlock_t */
	#define __spinlock_init_fn    /**< spinlock_init()    */
	#define __spinlock_lock_fn    /**< spinlock_lock()    */
	#define __spinlock_trylock_fn /**< spinlock_trylock() */
	#define __spinlock_unlock_fn  /**< spinlock_unlock()  */
	/**@}*/

	/**
	 * @name Spinlock State
	 */
	/**@{*/
	#define SPINLOCK_UNLOCKED LINUX64_SPINLOCK_UNLOCKED /**< @see LINUX64_SPINLOCK_UNLOCKED    */
	#define SPINLOCK_LOCKED   LINUX64_SPINLOCK_UNLOCKED /**< Cannot declare satic locked mutex */
	/**@}*/

	/**
	 * @see linux64_spinlock_t
	 */
	typedef linux64_spinlock_t spinlock_t;

	/**
	 * @see linux64_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		linux64_spinlock_init(lock);
	}

	/**
	 * @see linux64_spinlock_trylock().
	 *
	 * @note This operation performs a full data-cache flush in non-cache
	 * coherent processors.
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		int ret = linux64_spinlock_trylock(lock);

		linux64_core_dcache_invalidate();

		return (ret);
	}

	/**
	 * @see linux64_spinlock_lock().
	 *
	 * @note This operation performs a full data-cache flush in non-cache
	 * coherent processors.
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		linux64_spinlock_lock(lock);
	}

	/**
	 * @see linux64_spinlock_unlock().
	 *
	 * @note This operation performs a full data-cache flush in non-cache
	 * coherent processors.
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		linux64_spinlock_unlock(lock);
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_SPINLOCK_H_ */
