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

#ifndef ARCH_CORE_LINUX64_CORE_H_
#define ARCH_CORE_LINUX64_CORE_H_

	/* Must comme first. */
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup linux64-core
 */
/**@{*/

	#include <arch/core/linux64/types.h>
	#include <nanvix/const.h>
	#include <nanvix/klib.h>
	#include <pthread.h>

	#define LINUX64_CLUSTER_NUM_CORES 4
	
	EXTERN pthread_t linux64_cores_tab[LINUX64_CLUSTER_NUM_CORES];

	/**
	 * @brief Powers off the underlying core.
	 */
	EXTERN NORETURN void linux64_core_poweroff(void);

	/**
	 * @brief Gets the ID of the underlying core.
	 *
	 * @returns The ID of the underlying core or -1 if the thread
	 * calling thread is not attached to the underlying cluster.
	 */
	EXTERN int linux64_core_get_id(void);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64
 */

	/**
	 * @name Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   LINUX64_BYTE_BIT    /**< @ref BYTE_BIT   */
	#define HWORD_BIT  LINUX64_HWORD_BIT   /**< @ref HWORD_BIT  */
	#define WORD_BIT   LINUX64_WORD_BIT    /**< @ref WORD_BIT   */
	#define DWORD_BIT  LINUX64_DWORD_BIT   /**< @ref DWORD_BIT  */
	#define BYTE_SIZE  LINUX64_SIZE_SIZE   /**< @ref BYTE_SIZE  */
	#define HWORD_SIZE LINUX64_HWORD_SIZE  /**< @ref HWORD_SIZE */
	#define WORD_SIZE  LINUX64_WORD_SIZE   /**< @ref WORD_SIZE  */
	#define DWORD_SIZE LINUX64_DWORD_SIZE  /**< @ref DWORD_SIZE */
	/**@}*/

	/**
	 * @name Exported Types
	 */
	/**@{*/
	#define __byte_t  /**< @ref byte_t  */
	#define __hword_t /**< @ref hword_t */
	#define __word_t  /**< @ref word_t  */
	#define __dword_t /**< @ref dword_t */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __core_get_id_fn    /**< core_get_id()    */
	#define __core_poweroff_fn  /**< core_poweroff()  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef linux64_byte_t  byte_t;  /**< Byte        */
	typedef linux64_hword_t hword_t; /**< Half Word   */
	typedef linux64_word_t  word_t;  /**< Word        */
	typedef linux64_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @see linux64_core_get_id().
	 */
	static inline int core_get_id(void)
	{
		return (linux64_core_get_id());
	}

	/**
	 * @see linux64_core_poweroff().
	 */
	static inline NORETURN void core_poweroff(void)
	{
		linux64_core_poweroff();
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_CORE_H_ */
