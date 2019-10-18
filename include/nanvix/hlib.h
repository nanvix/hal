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

#ifndef NANVIX_HLIB
#define NANVIX_HLIB
#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <nanvix/klib.h>

/**
 * @defgroup klib Kernel Library
 * @ingroup kernel
 */

/*============================================================================*
 *                          Logging and Debugging                             *
 *============================================================================*/

/**
 * @addtogroup klib-debug Log & Debug
 * @ingroup klib
 */
/**@{*/

	/**
	 * @brief Kernel buffer size (in bytes).
	 *
	 * @note Hopefully not kernel string is longer than this.
	 */
	#define KBUFFER_SIZE 128

	/**
	 * @brief Prints a string on the standard output device.
	 *
	 * @param str Target string.
	 */
	EXTERN void kputs(const char *str);

	/**
	 * @brief Panics the kernel.
	 *
	 * @param fmt Panic message.
	 */
	EXTERN NORETURN void kpanic(const char *fmt, ...);

	/* Forward definitions. */
	EXTERN void kprintf(const char *, ...);
	EXTERN void kmemdump(const void *, size_t);

/**@}*/

/*============================================================================*
 *                              Miscellaneous                                 *
 *============================================================================*/

/**
 * @addtogroup klib-misc Miscellaneous
 * @ingroup klib
 */
/**@{*/

	/**
	 * @brief Kernel assert.
	 *
	 * The kassert() function asserts if @p expr evaluates to non-zero. If
	 * not, it panics the kernel with the @p msg.
	 *
	 * @see kpanic()
	 */
	static inline void _kassert(
		int expr,
		const char *msg,
		const char *filename,
		int linenum)
	{
		if (!expr)
		{
			kpanic(
				"%s (%s : %d)",
				(msg != NULL) ? msg : "",
				filename,
				linenum
			);

		}
	}

	/**
	 * @see _kassert()
	 */
	#ifdef NDEBUG
	#define kassert(x, msg) { UNUSED(x); UNUSED(msg); }
	#else
	#define kassert(x, msg) _kassert(x, msg, __FILE__, __LINE__)
	#endif

	/**
	 * @brief Wrapper for kassert().
	 *
	 * The KASSERT() is indeed a wrapper macro for kassert() where we
	 * do not want to print an error message. It is useful to assert
	 * parameters of kernel routines.
	 */
	#define KASSERT(x) kassert(x, "kassert() failed")

/**@}*/

#endif /* _ASM_FILE_ */
#endif /* NANVIX_HLIB */
