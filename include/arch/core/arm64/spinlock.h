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

#ifndef ARCH_CORE_ARM64_SPINLOCK_H_
#define ARCH_CORE_ARM64_SPINLOCK_H_

/**
 * @addtogroup arm64-core-spinlock Spinlock
 * @ingroup arm64-core
 *
 * @brief arm64 Spinlocks
 */
/**@{*/

#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <posix/stdint.h>

#endif /* _ASM_FILE_ */

#ifndef _ASM_FILE_

	/**
	 * @brief Spinlock.
	 */
	typedef uint64_t arm64_spinlock_t;

#endif

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond arm64
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
	#define SPINLOCK_UNLOCKED 0 /**< @see ARM64_SPINLOCK_UNLOCKED */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see arm64_spinlock_t
	 */
	typedef arm64_spinlock_t spinlock_t;

	/**
	 * @see arm64_spinlock_init().
	 */
	static inline void spinlock_init(spinlock_t *lock)
	{
		UNUSED(lock);
	}

	/**
	 * @see arm64_spinlock_trylock().
	 */
	static inline int spinlock_trylock(spinlock_t *lock)
	{
		UNUSED(lock);
		return 0;
	}

	/**
	 * @see arm64_spinlock_lock().
	 */
	static inline void spinlock_lock(spinlock_t *lock)
	{
		UNUSED(lock);
	}

	/**
	 * @see arm64_spinlock_unlock().
	 */
	static inline void spinlock_unlock(spinlock_t *lock)
	{
		UNUSED(lock);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_ARM64_SPINLOCK_H_ */
