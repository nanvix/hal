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

#ifndef ARCH_CORE_RV32GC_SPINLOCK_H_
#define ARCH_CORE_RV32GC_SPINLOCK_H_

/**
 * @addtogroup rv32gc-core-spinlock Spinlock
 * @ingroup rv32gc-core
 *
 * @brief rv32gc Spinlocks
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
	#define RV32GC_SPINLOCK_UNLOCKED 0x0 /**< Unlocked */
	#define RV32GC_SPINLOCK_LOCKED   0x1 /**< Locked   */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Spinlock.
	 */
	typedef uint32_t rv32gc_spinlock_t;

	/**
	 * @brief Initializes a spinlock.
	 *
	 * @param lock Target spinlock.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32gc_spinlock_init(rv32gc_spinlock_t *lock)
	{
		*lock = RV32GC_SPINLOCK_UNLOCKED;
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
	 *
	 * @todo Implement this function.
	 */
	static inline int rv32gc_spinlock_trylock(rv32gc_spinlock_t *lock)
	{
		return (
			!__sync_bool_compare_and_swap(
				lock,
				RV32GC_SPINLOCK_UNLOCKED,
				RV32GC_SPINLOCK_LOCKED
			)
		);
	}

	/**
	 * @brief Locks a spinlock.
	 *
	 * @param lock Target spinlock.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32gc_spinlock_lock(rv32gc_spinlock_t *lock)
	{
		while (rv32gc_spinlock_trylock(lock))
			/* noop */;

		__sync_synchronize();
	}

	/**
	 * @brief Unlocks a spinlock.
	 *
	 * @param lock Target spinlock.
	 *
	 * @todo Implement this function.
	 */
	static inline void rv32gc_spinlock_unlock(rv32gc_spinlock_t *lock)
	{
		*lock = RV32GC_SPINLOCK_UNLOCKED;
		__sync_synchronize();
	}

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond rv32gc
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
	#define SPINLOCK_UNLOCKED RV32GC_SPINLOCK_UNLOCKED /**< @see RV32GC_SPINLOCK_UNLOCKED */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see rv32gc_spinlock_t
	 */
	typedef rv32gc_spinlock_t spinlock_t;

	/**
	 * @see rv32gc_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		rv32gc_spinlock_init(lock);
	}

	/**
	 * @see rv32gc_spinlock_trylock().
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		return (rv32gc_spinlock_trylock(lock));
	}

	/**
	 * @see rv32gc_spinlock_lock().
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		rv32gc_spinlock_lock(lock);
	}

	/**
	 * @see rv32gc_spinlock_unlock().
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		rv32gc_spinlock_unlock(lock);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_RV32GC_SPINLOCK_H_ */
