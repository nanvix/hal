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

#ifndef ARCH_CORE_RI5CY_CORE_H_
#define ARCH_CORE_RI5CY_CORE_H_

	/* Must come first. */
	#define __NEED_CORE_REGS
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup ri5cy-core
 */
/**@{*/

	#include <arch/core/ri5cy/regs.h>
	#include <arch/core/ri5cy/types.h>

	/**
	 * @brief ID of the master core.
	 */
	#define RI5CY_COREID_MASTER (0x00 < RI5CY_MHARTID_COREID_SHIFT)

	/**
	 * @brief ID of the master cluster.
	 */
	#define RI5CY_CLUSTERID_MASTER (0x1f << RI5CY_MHARTID_CLUSTERID_SHIFT)

#ifndef _ASM_FILE_

	/**
	 * @brief Gets the ID of the underlying core.
	 */
	static inline int ri5cy_core_get_id(void)
	{
		ri5cy_word_t mhartid;

		mhartid = ri5cy_mhartid_read();

		return (RI5CY_MHARTID_COREID(mhartid));
	}

	/**
	 * @brief Gets the ID of the underlying cluster.
	 */
	static inline int ri5cy_cluster_get_id(void)
	{
		ri5cy_word_t mhartid;

		mhartid = ri5cy_mhartid_read();

		return (RI5CY_MHARTID_CLUSTERID(mhartid));
	}

#endif /* !_ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond ri5cy
 */

	/**
	 * @brief Exported Constants
	 */
	/**@{*/
	#define BYTE_BIT   RI5CY_BYTE_BIT   /**< @see BYTE_BIT   */
	#define HWORD_BIT  RI5CY_HWORD_BIT  /**< @see HWORD_BIT  */
	#define WORD_BIT   RI5CY_WORD_BIT   /**< @see WORD_BIT   */
	#define DWORD_BIT  RI5CY_DWORD_BIT  /**< @see DWORD_BIT  */
	#define BYTE_SIZE  RI5CY_SIZE_SIZE  /**< @see BYTE_SIZE  */
	#define HWORD_SIZE RI5CY_HWORD_SIZE /**< @see HWORD_SIZE */
	#define WORD_SIZE  RI5CY_WORD_SIZE  /**< @see WORD_SIZE  */
	#define DWORD_SIZE RI5CY_DWORD_SIZE /**< @see DWORD_SIZE */
	/**@}*/

	/**
	 * @brief Exported Types
	 */
	/**@{*/
	#define __byte_t  /**< @see byte_t  */
	#define __hword_t /**< @see hword_t */
	#define __word_t  /**< @see word_t  */
	#define __dword_t /**< @see dword_t */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __core_get_id_fn /**< core_get_id() */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @name Core Types
	 */
	/**@{*/
	typedef ri5cy_byte_t  byte_t;  /**< Byte        */
	typedef ri5cy_hword_t hword_t; /**< Half Word   */
	typedef ri5cy_word_t  word_t;  /**< Word        */
	typedef ri5cy_dword_t dword_t; /**< Double Word */
	/**@}*/

	/**
	 * @name Memory Types
	 */
	/**@{*/
	typedef ri5cy_paddr_t paddr_t; /**< Physical Address */
	typedef ri5cy_frame_t frame_t; /**< Frame Number     */
	typedef ri5cy_vaddr_t vaddr_t; /**< Virtual Address  */
	/**@}*/

	/**
	 * @see ri5cy_get_core_id
	 */
	static inline int core_get_id(void)
	{
		return (ri5cy_core_get_id());
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif	/* ARCH_CORE_RI5CY_CORE_H_ */
