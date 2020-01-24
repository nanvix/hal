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

#ifndef ARCH_CORE_I486_SPINLOCK_H_
#define ARCH_CORE_I486_SPINLOCK_H_

/**
 * @addtogroup i486-core-spinlock Spinlock
 * @ingroup i486-core
 *
 * @brief i486 Spinlocks
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
	#define I486_SPINLOCK_UNLOCKED 0x0 /**< Unlocked */
	#define I486_SPINLOCK_LOCKED   0x1 /**< Locked   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Spinlock.
	 */
	typedef uint32_t i486_spinlock_t;

	/**
	 * @brief Initializes a i486_spinlock_t.
	 *
	 * @param lock Target i486_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void i486_spinlock_init(i486_spinlock_t *lock)
	{
		*lock = I486_SPINLOCK_UNLOCKED;
	}

	/**
	 * @brief Attempts to lock a i486_spinlock_t.
	 *
	 * @param lock Target i486_spinlock_t.
	 *
	 * @returns Upon successful completion, the i486_spinlock_t pointed to by
	 * @p lock is locked and zero is returned. Upon failure, non-zero
	 * is returned instead, and the lock is not acquired by the
	 * caller.
	 *
	 * @todo Implement this function.
	 */
	static inline int i486_spinlock_trylock(i486_spinlock_t *lock)
	{
		return (
			!__sync_bool_compare_and_swap(
				lock,
				I486_SPINLOCK_UNLOCKED,
				I486_SPINLOCK_LOCKED
			)
		);
	}

	/**
	 * @brief Locks a i486_spinlock_t.
	 *
	 * @param lock Target i486_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void i486_spinlock_lock(i486_spinlock_t *lock)
	{
		while (i486_spinlock_trylock(lock))
			/* noop */;
		__sync_synchronize();
	}

	/**
	 * @brief Unlocks a i486_spinlock_t.
	 *
	 * @param lock Target i486_spinlock_t.
	 *
	 * @todo Implement this function.
	 */
	static inline void i486_spinlock_unlock(i486_spinlock_t *lock)
	{
		*lock = I486_SPINLOCK_UNLOCKED;
		__sync_synchronize();
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
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
	#define SPINLOCK_UNLOCKED I486_SPINLOCK_UNLOCKED /**< @see I486_SPINLOCK_UNLOCKED */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see i486_spinlock_t
	 */
	typedef i486_spinlock_t spinlock_t;

	/**
	 * @see i486_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		i486_spinlock_init(lock);
	}

	/**
	 * @see i486_spinlock_trylock().
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		return (i486_spinlock_trylock(lock));
	}

	/**
	 * @see i486_spinlock_lock().
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		i486_spinlock_lock(lock);
	}

	/**
	 * @see i486_spinlock_unlock().
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		i486_spinlock_unlock(lock);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_SPINLOCK_H_ */
