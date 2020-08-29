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

#ifndef NANVIX_HLIB
#define NANVIX_HLIB
#ifndef _ASM_FILE_

	#include <posix/stddef.h>
	#include <posix/stdarg.h>
	#include <nanvix/const.h>
	#include <nanvix/barelib.h>

/**
 * @defgroup hlib HAL Library
 * @ingroup hal
 */

/*============================================================================*
 * Memory Manipulation                                                        *
 *============================================================================*/

/**
 * @addtogroup hlib-memory
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @see __memchr().
	 */
	#define kmemchr(s,c,n) __memchr(s,c,n)

	/**
	 * @see __memcmp().
	 */
	#define kmemcmp(s1,s2,n) __memcmp(s1,s2,n)

	/**
	 * @see __memcpy().
	 */
	#define kmemcpy(s1,s2,n) __memcpy(s1,s2,n)

	/**
	 * @see __memmove().
	 */
	#define kmemmove(s1,s2,n) __memmove(s1,s2,n)

	/**
	 * @see __memset().
	 */
	#define kmemset(s,c,n) __memset(s,c,n)

/**@}*/

/*============================================================================*
 * String Manipulation                                                        *
 *============================================================================*/

/**
 * @addtogroup hlib-string String Manipulation
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @see __strcat().
	 */
	#define kstrcat(s1,s2) __strcat(s1,s2)

	/**
	 * @see __strchr().
	 */
	#define kstrchr(s,c) __strchr(s,c)

	/**
	 * @see __strcmp().
	 */
	#define kstrcmp(s1,s2) __strcmp(s1,s2)

	/**
	 * @see __strcpy().
	 */
	#define kstrcpy(s1,s2) __strcpy(s1,s2)

	/**
	 * @see __strcspn().
	 */
	#define kstrcspn(s1,s2) __strcspn(s1,s2)

	/**
	 * @see __strlen()
	 */
	#define kstrlen(str) __strlen(str)

	/**
	 * @see __strncat()
	 */
	#define kstrncat(s1,s2,n) __strncat(s1,s2,n)

	/**
	 * @see __strncmp().
	 */
	#define kstrncmp(s1,s2,n) __strncmp(s1,s2,n)

	/**
	 * @see __strncpy().
	 */
	#define kstrncpy(s1,s2,n) __strncpy(s1,s2,n)

	/**
	 * @see __strnlen().
	 */
	#define kstrnlen(s,maxlen) __strnlen(s,maxlen)

	/**
	 * @see __strpbrk().
	 */
	#define kstrpbrk(s1,s2) __strpbrk(s1,s2)

	/**
	 * @see __strrchr().
	 */
	#define kstrrchr(s,c) __strrchr(s,c)

	/**
	 * @see __strspn().
	 */
	#define kstrspn(s1,s2) __strspn(s1,s2)

	/**
	 * @see __strstr().
	 */
	#define kstrstr(s1,s2) __strstr(s1,s2)

/**@}*/

/*============================================================================*
 * String Formatting                                                          *
 *============================================================================*/

/**
 * @addtogroup hlib-formatting String Formatting
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @see __sprintf().
	 */
	#define ksprintf(str,fmt,...) __sprintf(str,fmt,__VA_ARGS__)

	/**
	 * @see __vsnprintf().
	 */
	#define kvsnprintf(str,size,fmt,args) __vsnprintf(str,size,fmt,args)
/**@}*/

/*============================================================================*
 *                          Logging and Debugging                             *
 *============================================================================*/

/**
 * @addtogroup hlib-debug Log & Debug
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @brief HAL buffer size (in bytes).
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
 * @addtogroup hlib-misc Miscellaneous
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @brief HAL assert.
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

/*============================================================================*
 * Bit-Wise Manipulation                                                      *
 *============================================================================*/

/**
 * @addtogroup hlib-bits Bit-Wise Manipulation
 * @ingroup hlib
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
 * Miscellaneous                                                              *
 *============================================================================*/

/**
 * @addtogroup hlib-misc Miscellaneous
 * @ingroup hlib
 */
/**@{*/

	/**
	 * @brief Multiplies two integers.
	 *
	 * @param a First operand.
	 * @param b Second operand.
	 *
	 * @returns The result of @p a times @p b.
	 */
	#if (__HAS_HW_MULTIPLICATION)
		#define mult(a, b) ((a)*(b))
	#else
		#define mult(a, b) __mult((a), (b))
	#endif

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
#endif /* NANVIX_HLIB */
