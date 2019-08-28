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

#ifndef NANVIX_KLIB_H_
#define NANVIX_KLIB_H_
#ifndef _ASM_FILE_

	#include <nanvix/const.h>
	#include <stdarg.h>

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
	EXTERN int kvsprintf(char *, const char *, va_list);
	EXTERN void kmemdump(const void *, size_t);

/**@}*/

/*============================================================================*
 *                                 String                                     *
 *============================================================================*/

/**
 * @addtogroup klib-string Strings
 * @ingroup klib
 */
/**@{*/

	/* Forward definitions. */
	EXTERN int kstrcmp(const char *, const char *);
	EXTERN char *kstrcpy(char *, const char *);
	EXTERN size_t kstrlen(const char *);
	EXTERN int kstrncmp(const char *, const char *, size_t);
	EXTERN char *kstrncpy(char *, const char *, size_t);

/**@}*/

/*============================================================================*
 * Bit-Wise Manipulation                                                      *
 *============================================================================*/

/**
 * @addtogroup klib-bits Bit-Wise Manipulation
 * @ingroup klib
 */
/**@{*/

	/**
	 * @brief Get bits of a bitmap.
	 *
	 * @param bits Bitmap.
	 * @param mask Mask.
	 */
	#define BITS_GET(bits, mask) \
		(((bits) & (mask)) / ((mask) & ~((mask)-1)))

	/**
	 * @brief      Set bits on a bitmap.
	 * @param mask Mask.
	 * @param val  Value.
	 */
	#define BITS_SET(bits, mask, val) \
		(((bits) & ~(mask)) | ((val) * ((mask) & ~((mask)-1))))

/**@}*/

/*============================================================================*
 *                                 Memory                                     *
 *============================================================================*/

/**
 * @addtogroup klib-memory Memory
 * @ingroup klib
 */
/**@{*/

	/* Forward definitions. */
	EXTERN void *kmemcpy(void *, const void *, size_t);
	EXTERN void *kmemset(void *, int, size_t);

	/**
	 * @brief Asserts if 'a' and 'b' agrees on size.
	 *
	 * @param a Probing size.
	 * @param b Control size.
	 *
	 * @returns Upon success, compilation proceeds as normal. Upon
	 * failure, a compilation error is generated.
	 */
	#define KASSERT_SIZE(a, b) \
		((void) sizeof(char[(((a) == (b)) ? 1 : -1)]))

	/**
	 * @brief Asserts if a value is aligned to a boundary.
	 *
	 * @param x Target value.
	 * @param a Boundary.
	 *
	 * @returns True if the target value is aligned on the given
	 * boundary, and false otherwise.
	 */
	#define ALIGNED(x, a) \
		(!((x) & ((a) - 1)))

	/**
	 * @brief Truncate a value on a boundary.
	 *
	 * @param x Value to be aligned.
	 * @param a Boundary.
	 *
	 * @returns Truncated value.
	 */
	#define TRUNCATE(x, a) \
		(((x) + ((a) - 1)) & ~((a) - 1))

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

	/**
	 * @brief Returns the length of an array.
	 *
	 * @param x Target array.
	 *
	 * @returns The length of the array @p x.
	 */
	#define ARRAY_LENGTH(x) \
		(sizeof(x)/sizeof((x)[0]))

	/**
	 * @brief Asserts if a number is within a range.
	 *
	 * @param x Number.
	 * @param a Start of range.
	 * @param b End of range.
	 *
	 * @returns Non-zero if @p x is within [a, b) and zero otherwise.
	 */
	#define WITHIN(x, a, b) (((x) >= (a)) && ((x) < (b)))

	/**
	 * @brief Concatenates two macros.
	 *
	 * @param x First macro.
	 * @param y Second macro.
	 */
	#define CONCAT2(x, y) x ## y

	/**
	 * @brief Expands a macro.
	 *
	 * @param x First macro.
	 * @param y Second macro.
	 */
	#define EXPAND2(x, y) CONCAT2(x, y)

	/**
	 * @brief Auto-name for reserved fields in a structure.
	 */
	#define RESERVED EXPAND2(reserved, __LINE__)

/**@}*/

#endif /* _ASM_FILE_ */
#endif /* NANVIX_KLIB_H_ */
