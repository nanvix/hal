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

#ifndef ARCH_I486_IDT_H_
#define ARCH_I486_IDT_H_

/**
 * @addtogroup i486-core-idt IDT
 * @ingroup i486-core
 *
 * @brief Interrupt Descritor Table
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Size of and IDT entry (in bytes).
	 */
	#define IDTE_SIZE 8

	/**
	 * @brief Size of IDTPTR (in bytes).
	 */
	#define IDTPTR_SIZE 6

	/**
	 * @brief Number of entries in the IDT.
	 */
	#define IDT_SIZE 256

	/**
	 * @name Types of Interrupt Entries
	 */
	/**@{*/
	#define IDT_TASK32 0x5 /**< 32-bit task gate.      */
	#define IDT_INT16  0x6 /**< 16-bit interrupt gate. */
	#define IDT_TRAP16 0x7 /**< 16-bit trap gate.      */
	#define IDT_INT32  0xe /**< 32-bit interrupt gate. */
	#define IDT_TRAP32 0xf /**< 32-bit trap gate.      */
	/**@}*/

	/**
	 * @name Software Interrupt Hooks
	 */
	/**@{*/
	EXTERN void _i486_do_excp0(void);  /**< Division-by-Zero Error      */
	EXTERN void _i486_do_excp1(void);  /**< Debug Exception             */
	EXTERN void _i486_do_excp2(void);  /**< Non-Maskable Interrupt      */
	EXTERN void _i486_do_excp3(void);  /**< Breakpoint Exception        */
	EXTERN void _i486_do_excp4(void);  /**< Overflow Exception          */
	EXTERN void _i486_do_excp5(void);  /**< Bounds Check Exception      */
	EXTERN void _i486_do_excp6(void);  /**< Invalid Opcode Exception    */
	EXTERN void _i486_do_excp7(void);  /**< Coprocessor Not Available   */
	EXTERN void _i486_do_excp8(void);  /**< Double Fault                */
	EXTERN void _i486_do_excp9(void);  /**< Coprocessor Segment Overrun */
	EXTERN void _i486_do_excp10(void); /**< Invalid TSS                 */
	EXTERN void _i486_do_excp11(void); /**< Segment Not Present         */
	EXTERN void _i486_do_excp12(void); /**< Stack Segment Fault         */
	EXTERN void _i486_do_excp13(void); /**< General Protection Fault    */
	EXTERN void _i486_do_excp14(void); /**< Page Fault                  */
	EXTERN void _i486_do_excp15(void); /**< Reserved                    */
	EXTERN void _i486_do_excp16(void); /**< Floating Point Exception    */
	EXTERN void _i486_do_excp17(void); /**< Alignment Check Exception   */
	EXTERN void _i486_do_excp18(void); /**< Machine Check Exception     */
	EXTERN void _i486_do_excp19(void); /**< SMID Unit Exception         */
	EXTERN void _i486_do_excp20(void); /**< Virtual Exception           */
	EXTERN void _i486_do_excp30(void); /**< Security Exception.         */
	/**@}*/

	/**
	 * @brief Interrupt descriptor table entry.
	 */
	struct idte
	{
		unsigned handler_low  : 16; /**< Handler low.           */
		unsigned selector     : 16; /**< GDT selector.          */
		unsigned              :  8; /**< Always zero.           */
		unsigned type         :  4; /**< Gate type (sse above). */
		unsigned flags        :  4; /**< Flags.                 */
		unsigned handler_high : 16; /**< handler high.          */
	} __attribute__((packed));

	/**
	 * @brief Interrupt descriptor table pointer.
	 */
	struct idtptr
	{
		unsigned size : 16; /**< IDT size.            */
		unsigned ptr  : 32; /**< IDT virtual address. */
	} __attribute__((packed));

	/**
	 * Initializes the Interrupt Descriptor Table (IDT).
	 */
	EXTERN void idt_setup(void);

	/**
	 * @brief Flushes the IDT.
	 *
	 * Flushes the Interrupt Descriptor Table (IDT).
	 *
	 * @param idtptr Interrupt descriptor table pointer.
	 */
	static inline void idt_flush(const struct idtptr *idtptr)
	{
		asm volatile (
			"mov %0, %%eax;\
			lidt (%%eax);"
			:
			: "r" (idtptr)
			: "memory", "eax"
		);
	}

/**@}*/

#endif /* ARCH_I486_IDT_H_ */
