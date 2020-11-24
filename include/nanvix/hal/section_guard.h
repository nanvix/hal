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

/*============================================================================*
 * Section Guard Interface                                                    *
 *============================================================================*/

/**
 * @addtogroup kernel-hal-cluster-section-guard Section Guard
 * @ingroup kernel-hal-cluster
 *
 * @brief Section Guard
 */
/**@{*/

#ifdef __NEED_SECTION_GUARD
#ifndef NANVIX_HAL_SECTION_GUARD_H_
#define NANVIX_HAL_SECTION_GUARD_H_

	#include <nanvix/hal/core/interrupt.h>
	#include <nanvix/const.h>

	/**
	 * @brief Initializer for a resource.
	 */
	/**@{*/
	#define SECTION_GUARD_INITIALIZER(lock, level) (struct section_guard) { level, lock }
	/**@}*/

	/**
	 * @brief Critical section guard.
	 *
	 * @details This struct protects a critical section with extra information.
	 * In addition to securing the section with a lock, it guarantees that the
	 * execution context will not be interrupted by low-priority interrupts
	 * defined by level field.
	 *
	 * This struct is not thread-safe.
	 * This struct is not reused (the same of not reentry function).
	 * This struct must be stored into stack.
	 */
	struct section_guard
	{
		bool changed;      /**< Signals if we change the interrupt level. */
		int level;         /**< Interrupt level.                          */
		spinlock_t * lock; /**< Critical region lock.                     */
	};

	/**
	 * @brief Initializes a section guard.
	 *
	 * @param guard Section guard.
	 * @param lock  Lock of the critical section.
	 * @param level Interrupt level required.
	 */
	static inline void section_guard_init(
		struct section_guard * guard,
		spinlock_t * lock,
		int level
	)
	{
		guard->changed = false;
		guard->level   = level;
		guard->lock    = lock;
	}

	/**
	 * @brief Entry to a critical section.
	 *
	 * @param guard Section guard.
	 */
	static inline void section_guard_entry(struct section_guard * guard)
	{
		/* Does the current level has a lower priority than the new one? */
		guard->changed = (guard->level > interrupts_get_level());

		/* Sets new interrupt level and keeps the old level. */
		if (guard->changed)
			KASSERT((guard->level = interrupts_set_level(guard->level)) >= 0);

		/* Entry in the critical section. */
		spinlock_lock(guard->lock);
	}

	/**
	 * @brief Exit from a critical section.
	 *
	 * @param guard Section guard.
	 */
	static inline void section_guard_exit(struct section_guard * guard)
	{
		/* Exit in the critical section. */
		spinlock_unlock(guard->lock);

		/* Restore interrupt level. */
		if (guard->changed)
			KASSERT((guard->level = interrupts_set_level(guard->level)) >= 0);

		/* Restore signal. */
		guard->changed = false;
	}

/**@}*/

#endif /* NANVIX_HAL_SECTION_GUARD_H_ */
#endif /* __NEED_SECTION_GUARD */
