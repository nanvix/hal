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

#ifndef ARCH_CORE_RV32GC_SREGS_H_
#define ARCH_CORE_RV32GC_SREGS_H_

	#ifndef __NEED_CORE_SREGS
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32gc-core-sregs Supervisor Registers
 * @ingroup rv32gc-core-regs
 *
 * @brief Supervisor Registers
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/rv32gc/types.h>

	/**
	 * @name Supervisor Address Translation and Protection Register (stap)
	 */
	/**@{*/
	#define RV32_SATP_MODE_SV32 (1 << 31) /**< Page-Based 32-Bit Virtual Addressing */
	/**@}*/

	/**
	 * @brief Supervisor Trap Setup Registers
	 */
	/**@{*/
	#define RV32GC_CSR_SSTATUS    0x100 /**< Supervisor Status           */
	#define RV32GC_CSR_SIE        0x104 /**< Supervisor Interrupt Enable */
	#define RV32GC_CSR_STVEC      0x105 /**< Supervisor Trap Vector      */
	#define RV32GC_CSR_SCOUNTEREN 0x106 /**< Supervisor Counter-Enable   */
	/**@}*/

	/**
	 * @brief Supervisor Trap Handling Registes
	 */
	/**@{*/
	#define RV32GC_CSR_SSCRATCH 0x140 /**< Supervisor Scratch                   */
	#define RV32GC_CSR_SEPC     0x141 /**< Supervisor Exception Program Counter */
	#define RV32GC_CSR_SCAUSE   0x142 /**< Supervisor Exception Cause           */
	#define RV32GC_CSR_STVAL    0x143 /**< Supervisor Trap Value                */
	#define RV32GC_CSR_SIP      0x144 /**< Supervisor Interrupt Pending         */
	/**@}*/

	/**
	 * @name Supervisor Protection and Translation Registers
	 */
	/**@{*/
	#define RV32GC_CSR_SATP 0x180 /**< Supervisor Address Translation and Protection */
	/**@}*/

	/**
	 * @name Supervisor Status Register (sstatus)
	 */
	/**@{*/
	#define RV32GC_SSTATUS_UIE   (0x1 <<  0) /**< User Interrupt Enable                */
	#define RV32GC_SSTATUS_SIE   (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32GC_SSTATUS_UPIE  (0x1 <<  4) /**< User Previous Interrupt Enable       */
	#define RV32GC_SSTATUS_SPIE  (0x1 <<  5) /**< Supervisor Previous Interrupt Enable */
	#define RV32GC_SSTATUS_SPP   (0x1 <<  8) /**< Supervisor Previous Priviledge       */
	#define RV32GC_SSTATUS_SUM   (0x1 << 18) /**< Allow Supervisor User Memory Access  */
	#define RV32GC_SSTATUS_MXR   (0x1 << 19) /**< Make Executable                      */
	#define RV32GC_SSTATUS_SD    (0x1 << 31) /**< State Dirty                          */
	/**@}*/

	/**
	 * @name Supervisor Interrupt Enable Register (sie)
	 */
	/**@{*/
	#define RV32GC_SIE_USIE (0x1 <<  0) /**< User Software Interrupt Enable       */
	#define RV32GC_SIE_SSIE (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32GC_SIE_UTIE (0x1 <<  4) /**< User Timer Interrupt Enable          */
	#define RV32GC_SIE_STIE (0x1 <<  5) /**< Supervisor Timer Interrupt Enable    */
	#define RV32GC_SIE_UEUE (0x1 <<  8) /**< User External Interrupt Enable       */
	#define RV32GC_SIE_SEIE (0x1 <<  9) /**< Supervisor External Interrupt Enable */
	/**@}*/

	/**
	 * @name Supervisor Interrupt Pending (sip)
	 */
	/**@{*/
	#define RV32GC_SIP_USIP (0x1 << 0) /**< User Software Interrupt Pending       */
	#define RV32GC_SIP_SSIP (0x1 << 1) /**< Supervisor Software Interrupt Pending */
	#define RV32GC_SIP_UTIP (0x1 << 4) /**< User Timer Interrupt Pending          */
	#define RV32GC_SIP_STIP (0x1 << 5) /**< Supervisor Timer Interrupt Pending    */
	#define RV32GC_SIP_UEIP (0x1 << 8) /**< User External Interrupt Pending       */
	#define RV32GC_SIP_SEIP (0x1 << 9) /**< Supervisor External Interrupt Pending */
	/**@}*/

	/**
	 * @name Machine Cause Register (mcause)
	 */
	/**@{*/
	#define RV32GC_SCAUSE_INT   0x80000000 /**< Interrupt? */
	#define RV32GC_SCAUSE_CAUSE 0x0000000f /**< Trap Cause */
	/**@}*/

	/**
	 * @name Exception Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32GC_SCAUSE_ALIGN_I      0 /**< Instruction Address Misaligned */
	#define RV32GC_SCAUSE_PG_PROT_I    1 /**< Instruction Access Fault       */
	#define RV32GC_SCAUSE_ILLEGAL_I    2 /**< Illegal Instruction            */
	#define RV32GC_SCAUSE_BREAK        3 /**< Breakpoint                     */
	#define RV32GC_SCAUSE_PG_PROT_L    5 /**< Load Access Fault              */
	#define RV32GC_SCAUSE_ALIGN_S      6 /**< Store/AMO Address Misaligned   */
	#define RV32GC_SCAUSE_PG_PROT_S    7 /**< Store/AMO Access Fault         */
	#define RV32GC_SCAUSE_ECALL_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32GC_SCAUSE_PG_FAULT_I  12 /**< Instruction Page Fault         */
	#define RV32GC_SCAUSE_PG_FAULT_L  13 /**< Load Page Fault                */
	#define RV32GC_SCAUSE_PG_FAULT_S  15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32GC_SCAUSE_USI 0 /**< User Software Interrupt        */
	#define RV32GC_SCAUSE_SSI 1 /**< Supervisor Software Interrupt  */
	#define RV32GC_SCAUSE_UTI 4 /**< User Timer Interrupt           */
	#define RV32GC_SCAUSE_STI 5 /**< Supervisor Timer Interrupt     */
	#define RV32GC_SCAUSE_UEI 8 /**< User External Interrupt        */
	#define RV32GC_SCAUSE_SEI 9 /**< Supervisor External Interrupt  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Reads the sstatus register.
	 *
	 * @returns The value of the sstatus register.
	 */
	static inline rv32gc_word_t rv32gc_sstatus_read(void)
	{
		rv32gc_word_t sstatus;

		__asm__ __volatile__(
			"csrr %0, sstatus"
			: "=r" (sstatus)
		);

		return (sstatus);
	}

	/**
	 * @brief Writes to the sstatus register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_sstatus_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw sstatus, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the sie register.
	 *
	 * @returns The value of the sie register.
	 */
	static inline rv32gc_word_t rv32gc_sie_read(void)
	{
		rv32gc_word_t sie;

		__asm__ __volatile__(
			"csrr %0, sie"
			: "=r" (sie)
		);

		return (sie);
	}

	/**
	 * @brief Writes to the sie register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_sie_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw sie, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the sip register.
	 *
	 * @returns The value of the sip register.
	 */
	static inline rv32gc_word_t rv32gc_sip_read(void)
	{
		rv32gc_word_t sip;

		__asm__ __volatile__(
			"csrr %0, sip"
			: "=r" (sip)
		);

		return (sip);
	}

	/**
	 * @brief Writes to the sip register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_sip_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw sip, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the stvec register.
	 *
	 * @returns The value of the stvec register.
	 */
	static inline rv32gc_word_t rv32gc_stvec_read(void)
	{
		rv32gc_word_t stvec;

		__asm__ __volatile__(
			"csrr %0, stvec"
			: "=r" (stvec)
		);

		return (stvec);
	}

	/**
	 * @brief Writes to the stvec register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_stvec_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw stvec, %0"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the sepc register.
	 *
	 * @returns The value of the sepc register.
	 */
	static inline rv32gc_word_t rv32gc_sepc_read(void)
	{
		rv32gc_word_t sepc;

		__asm__ __volatile__(
			"csrr %0, sepc"
			: "=r" (sepc)
		);

		return (sepc);
	}

	/**
	 * @brief Writes to the sepc register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_sepc_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw sepc, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the scause register.
	 *
	 * @returns The value of the scause register.
	 */
	static inline rv32gc_word_t rv32gc_scause_read(void)
	{
		rv32gc_word_t scause;

		__asm__ __volatile__(
			"csrr %0, scause"
			: "=r" (scause)
		);

		return (scause);
	}

	/**
	 * @brief Reads the stval register.
	 *
	 * @returns The value of the stval register.
	 */
	static inline rv32gc_word_t rv32gc_stval_read(void)
	{
		rv32gc_word_t stval;

		__asm__ __volatile__(
			"csrr %0, stval"
			: "=r" (stval)
		);

		return (stval);
	}

	/**
	 * @brief Reads the satp register.
	 *
	 * @returns The value of the satp register.
	 */
	static inline rv32gc_word_t rv32gc_satp_read(void)
	{
		rv32gc_word_t satp;

		__asm__ __volatile__(
			"csrr %0, satp"
			: "=r" (satp)
		);

		return (satp);
	}

	/**
	 * @brief Writes to the satp register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_satp_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw satp, %0;"
			:
			: "r" (val)
		);
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32GC_SREGS_H_ */
