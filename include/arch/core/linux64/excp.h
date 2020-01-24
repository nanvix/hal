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

#ifndef ARCH_CORE_LINUX64_EXCP_H_
#define ARCH_CORE_LINUX64_EXCP_H_

	#include <signal.h>

	/* Must come first. */
	#define __NEED_CORE_CONTEXT
	#define __NEED_CORE_TYPES
	#define __NEED_MEMORY_TYPES

/**
 * @addtogroup linux64-core-exception Exception
 * @ingroup linux64-core
 *
 * @brief Exceptions
 */
/**@{*/

	/**
	 * @brief Exception information size (in bytes).
	 */
	#define LINUX64_EXCP_SIZE 8

	/**
	* @brief Max number of exceptions in the linux64 core.
	 */
	#define LINUX64_EXCP_NUM SIGSEGV + 1

	#define LINUX64_EXCP_INVALID_OPCODE		SIGILL
	#define LINUX64_EXCP_PAGE_FAULT			SIGSEGV
	#define LINUX64_EXCP_TLB_FAULT			SIGSEGV
	#define LINUX64_EXCP_GENERAL_PROTECTION	SIGSEGV

	/**
	 * @cond linux64
	 */

		/**
	 	* @brief Exception information.
	 	*/
		struct exception
		{
			unsigned num;               /**< Exception number. */
			linux64_byte_t RESERVED[4]; /**< Required padding. */
		} PACK;

	/**@endcond*/

	/**
	 * @brief Gets the number of an exception.
	 *
	 * The linux64_excp_get_num() function gets the exception number
	 * stored in the exception information sC_SRC += core/exception.ctructure pointed to by @p
	 * excp.
	 *
	 * @param excp Target exception information structure.
	 *
	 * @returns The exception number stored in the exception
	 * information structure pointed to by @p excp.
	 *
	 * @author Daniel Coscia
	 */
	static inline int linux64_excp_get_num(const struct exception *excp)
	{
		return (excp->num);
	}

	/**
	 * @brief Dumps information about an exception.
	 *
	 * @param excp Exception information.
	 */
	EXTERN void linux64_excp_dump(const struct exception *excp);

	/**
 	 * @brief Setup the signals
 	 */
	EXTERN void linux64_excp_setup(void);


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
	#define EXCEPTIONS_NUM                LINUX64_EXCP_NUM					/**< @ref LINUX64_EXCP_NUM             */
	#define EXCEPTION_SIZE                LINUX64_EXCP_SIZE					/**< @ref LINUX64_EXCP_SIZE            */
	#define EXCEPTION_INVALID_OPCODE      LINUX64_EXCP_INVALID_OPCODE		/**< @ref LINUX64_EXCP_INVALID_OPCODE  */
	#define EXCEPTION_PAGE_FAULT          LINUX64_EXCP_PAGE_FAULT			/**< @ref LINUX64_EXCP_VIRT_PAGE_FAULT */
	#define EXCEPTION_PAGE_PROTECTION     LINUX64_EXCP_PAGE_FAULT			/**< @ref LINUX64_EXCP_PAGE_PROTECTION */
	#define EXCEPTION_ITLB_FAULT          LINUX64_EXCP_TLB_FAULT			/**< @ref LINUX64_EXCP_TLB_FAULT       */
	#define EXCEPTION_DTLB_FAULT          LINUX64_EXCP_TLB_FAULT			/**< @ref LINUX64_EXCP_TLB_FAULT       */
	#define EXCEPTION_GENERAL_PROTECTION  LINUX64_EXCP_GENERAL_PROTECTION	/**< @ref LINUX64_EXCP_PROTECTION      */
	/**@}*/

	/**
	 * @name Exported Structures
	 */
	/**@{*/
	#define __exception_struct      /**< @ref exception */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __exception_get_addr_fn  /**< @ref exception_get_addr()  */
	#define __exception_get_instr_fn /**< @ref exception_get_instr() */
	#define __exception_get_num_fn   /**< @ref exception_get_num()   */
	#define __exception_dump_fn      /**< @ref exception_dump()      */
	/**@}*/

	/**
	 * @name Exported Variables
	 */
	/**@{*/
	#define __exceptions_var /**< @ref exceptions */
	/**@}*/

	/**
	 * @see linux64_excp_get_num().
	 */
	static inline int exception_get_num(const struct exception *excp)
	{
		return (linux64_excp_get_num(excp));
	}

	/**
	 * @brief Dummy operation.
	 *
	 * @param excp Unused parameter.
	 *
	 * @returns Always zero.
	 */
	static inline vaddr_t exception_get_addr(const struct exception *excp)
	{
		UNUSED(excp);

		return (0);
	}

	/**
	 * @brief Dummy operation.
	 *
	 * @param excp Unused parameter.
	 *
	 * @returns Always zero.
	 */
	static inline int exception_get_instr(const struct exception *excp)
	{
		UNUSED(excp);

		return (0);
	}

	/**
	 * @see linux64_excp_dump().
	 */
	static inline void exception_dump(const struct exception *excp)
	{
		linux64_excp_dump(excp);
	}

/**@endcond*/

#endif /* ARCH_CORE_LINUX64_EXCP_H_ */
