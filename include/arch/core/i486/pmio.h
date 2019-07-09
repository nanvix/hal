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

#ifndef ARCH_CORE_I486_PMIO_H_
#define ARCH_CORE_I486_PMIO_H_

/**
 * @addtogroup i486-core-io Port-Mapped I/O
 * @ingroup i486-core
 *
 * @brief Port-Mapped I/O
 */
/**@{*/

	#include <nanvix/const.h>
	#include <sys/types.h>
	#include <stdint.h>

#ifndef _ASM_FILE_

	/**
	 * @brief Writes 8 bits to an I/O port.
	 *
	 * @param port Number of the target port.
	 * @param bits Bits to write.
	 */
	static inline void i486_output8(uint16_t port, uint8_t bits)
	{
		asm volatile ("outb %0, %1" : : "a"(bits), "Nd"(port));
	}

	/**
	 * @brief Writes 16 bits to an I/O port.
	 *
	 * @param port Number of the target port.
	 * @param bits Bits to write.
	 */
	static inline void i486_output16(uint16_t port, uint16_t bits)
	{
		asm volatile ("outw %0, %1" : : "a"(bits), "Nd"(port));
	}

	/**
	 * @brief Writes 32 bits to an I/O port.
	 *
	 * @param port Number of the target port.
	 * @param bits Bits to write.
	 */
	static inline void i486_output32(uint16_t port, uint32_t bits)
	{
		asm volatile ("outl %0, %1" : : "a"(bits), "Nd"(port));
	}

	/**
	 * @brief Waits for an operation in an I/O port to complete.
	 */
	static inline void i486_iowait(void)
	{
		asm volatile ("outb %%al, $0x80" : : "a"(0));
	}

	/**
	 * @brief Writes a 8-bit string to an I/O port.
	 *
	 * @param port Number of the target port.
	 * @param str  8-bit string to write.
	 * @param len  Length of the string.
	 *
	 * @todo This can be platform-independent.
	 */
	static inline void i486_output8s(uint16_t port, const uint8_t *str, size_t len)
	{
		for (size_t i = 0; i < len; i++)
		{
			i486_output8(port, str[i]);
			i486_iowait();
		}
	}

	/**
	 * @brief Read 8 bits from an I/O port.
	 *
	 * @param port Number of the target port.
	 */
	static inline uint8_t i486_input8(uint16_t port)
	{
		uint8_t ret;
		asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
		return ret;
	}

	/**
	 * @brief Read 16 bits from an I/O port.
	 *
	 * @param port Number of the target port.
	 */
	static inline uint16_t i486_input16(uint16_t port)
	{
		uint16_t ret;
		asm volatile ("inw %1, %0" : "=a" (ret) : "Nd" (port));
		return ret;
	}

	/**
	 * @brief Read 32 bits from an I/O port.
	 *
	 * @param port Number of the target port.
	 */
	static inline uint32_t i486_input32(uint16_t port)
	{
		uint32_t ret;
		asm volatile ("inl %1, %0" : "=a" (ret) : "Nd" (port));
		return ret;
	}

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond i486
 */

	/**
	 * @name Provided Functions
	 */
	/**@{*/
	#define __output8_fn  /**< i486_output8()  */
	#define __output8s_fn /**< i486_output8s() */
	#define __iowait_fn   /**< iowait()        */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @see i486_output8().
	 */
	static inline void output8(uint16_t port, uint8_t bits)
	{
		i486_output8(port, bits);
	}

	/**
	 * @see i486_output16().
	 */
	static inline void output16(uint16_t port, uint16_t bits)
	{
		i486_output16(port, bits);
	}

	/**
	 * @see i486_output32().
	 */
	static inline void output32(uint16_t port, uint32_t bits)
	{
		i486_output32(port, bits);
	}

	/**
	 * @see i486_output8s().
	 */
	static inline void output8s(uint16_t port, const uint8_t *str, size_t len)
	{
		i486_output8s(port, str, len);
	}

	/**
	 * @see i486_iowait().
	 */
	static inline void iowait(uint16_t port)
	{
		((void) port);

		i486_iowait();
	}

	/**
	 * @see i486_input8().
	 */
	static inline uint8_t input8(uint16_t port)
	{
		return i486_input8(port);
	}

	/**
	 * @see i486_input16().
	 */
	static inline uint16_t input16(uint16_t port)
	{
		return i486_input16(port);
	}

	/**
	 * @see i486_input32().
	 */
	static inline uint32_t input32(uint16_t port)
	{
		return i486_input32(port);
	}

#endif /* _ASM_FILE_ */

/**@endcond*/

#endif /* ARCH_CORE_I486_PMIO_H_ */

