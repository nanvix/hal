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

/* Must come first. */
#define __NEED_HAL_CORE

#include <nanvix/hal/core.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/*============================================================================*
 * context_create()                                                           *
 *============================================================================*/

/**
 * @brief Create a context.
 *
 * @param start  Start routine.
 * @param ustack User stack pointer.
 * @param kstack Kernel stack pointer.
 *
 * @returns Context struct create into the kernel stack.
 *
 * @author João Vicente Souto
 */
PUBLIC struct context * context_create(
	void (* start)(void),
	struct stack * ustack,
	struct stack * kstack
)
{
	if ((start == NULL) || (ustack == NULL) || (kstack == NULL))
		return (NULL);

#if CORE_SUPPORTS_MULTITHREADING
	return (__context_create(start, ustack, kstack));
#else
	return (NULL);
#endif
}

/*============================================================================*
 * context_switch_to()                                                        *
 *============================================================================*/

/**
 * @brief Switch between contexts.
 *
 * @details The previous context will be store into kernel stack
 * and the pointer to it will be store into @previous parameter.
 * And the next context will be restore from the @next parameter.
 *
 * @param previous Pointer to store the pointer of the previous context.
 * @param next     Pointer to get the pointer to the next context.
 *
 * @returns Zero if the context was switched and restored, non-zero if
 * the arguments are invalid.
 *
 * @author João Vicente Souto
 */
PUBLIC int context_switch_to(struct context ** previous, struct context ** next)
{
	/* Is the previous pointer invalid? */
	if (previous == NULL)
		return (-EINVAL);

	/* Is the next pointer invalid? */
	if (next == NULL || (*next) == NULL)
		return (-EINVAL);

#if CORE_SUPPORTS_MULTITHREADING

	/* Restore contex. */
	__context_switch_to(previous, next);

	return (0);

#else
	return (-ENOSYS);
#endif
}

