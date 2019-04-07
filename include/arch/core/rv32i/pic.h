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

#ifndef ARCH_CORE_RV32I_PIC_H_
#define ARCH_CORE_RV32I_PIC_H_

	#ifndef __NEED_CORE_PIC
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32i-core-pic PIC
 * @ingroup rv32i-core
 *
 * @brief Programmable Interrupt Controller
 */
/**@{*/

	/**
	 * @name PIC Memory Map
	 */
	/**@{*/
	#define PIC_BASE            0x02000000 /** Base Address                   */
	#define PIC_MSIP            0x00000000 /**< Offset for msip Register      */
	#define PIC_MTIMECMP_OFFSET 0x00004000 /**< Offset for mtimecmp Register  */
	#define PIC_MTIME_OFFSET    0x0000bff8 /**< Offset for mtime Register     */
	/**@}*/

/**@}*/

#endif /* ARCH_CORE_RV32I_PIC_H_  */
