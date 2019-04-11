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

#ifndef ARCH_CORE_RV32GC_MREGS_H_
#define ARCH_CORE_RV32GC_MREGS_H_

	#ifndef __NEED_CORE_MREGS
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32gc-core-sregs Machine Registers
 * @ingroup rv32gc-core-regs
 *
 * @brief Machine Registers
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/rv32gc/types.h>

	/**
	 * @name Machine Information Registers
	 */
	/**@{*/
	#define RV32GC_CSR_MVENDORID 0xf11 /**< Machine Vendor ID         */
	#define RV32GC_CSR_MARCHID   0xf12 /**< Marchine Architecture ID  */
	#define RV32GC_CSR_MIMPID    0xf13 /**< Machine ID Implementation */
	#define RV32GC_CSR_MHARTID   0xf14 /**< Machine Hart ID           */
	/**@}*/

	/**
	 * @name Machine Trap Setup Registers
	 */
	/**@{*/
	#define RV32GC_CSR_MSTATUS    0x300 /**< Machine Status               */
	#define RV32GC_CSR_MISA       0x301 /**< Machine ISA and Extensions   */
	#define RV32GC_CSR_MEDELEG    0x302 /**< Machine Exception Delegation */
	#define RV32GC_CSR_MIDELEG    0x303 /**< Machine Interrupt Delegation */
	#define RV32GC_CSR_MIE        0x304 /**< Machine Interrupt Enable     */
	#define RV32GC_CSR_MTVEC      0x305 /**< Machine Trap Vector          */
	#define RV32GC_CSR_MCOUNTEREN 0x306 /**< Machine Counter-Enable       */
	/**@}*/

	/**
	 * @name Machine Trap Handling Registers
	 */
	/**@{*/
	#define RV32GC_CSR_MSCRATCH 0x340 /**< Machine Scratch                   */
	#define RV32GC_CSR_MEPC     0x341 /**< Machine Exception Program Counter */
	#define RV32GC_CSR_MCAUSE   0x342 /**< Machine Exception Cause           */
	#define RV32GC_CSR_MTVAL    0x343 /**< Machine Trap Value                */
	#define RV32GC_CSR_MIP      0x344 /**< Machine Interrupt Pending         */
	/**@}*/

	/**
	 * @name Privilege Modes
	 */
	/**@{*/
	#define RV32GC_PRV_U 0 /**< User/Application */
	#define RV32GC_PRV_S 1 /**< Supervisor       */
	#define RV32GC_PRV_M 3 /**< Machine          */
	/**@}*/

	/**
	 * @name Machine Status Register (mstatus)
	 */
	/**@{*/
	#define RV32GC_MSTATUS_UIE  (0x1 <<  0) /**< User Interrupt Enable                */
	#define RV32GC_MSTATUS_SIE  (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32GC_MSTATUS_MIE  (0x1 <<  3) /**< Machine Interrupt Enable             */
	#define RV32GC_MSTATUS_UPIE (0x1 <<  4) /**< User Previous Interrupt Enable       */
	#define RV32GC_MSTATUS_SPIE (0x1 <<  5) /**< Supervisor Previous Interrupt Enable */
	#define RV32GC_MSTATUS_MPIE (0x1 <<  7) /**< Machine Previous Interrupt Enable    */
	#define RV32GC_MSTATUS_SPP  (0x1 <<  8) /**< Supervisor Previous Priviledge       */
	#define RV32GC_MSTATUS_MPP  (0x3 << 11) /**< Machine Previous Priviledge          */
	#define RV32GC_MSTATUS_SUM  (0x1 << 18) /**< Allow Supervisor User Memory Access  */
	#define RV32GC_MSTATUS_MXR  (0x1 << 19) /**< Make Executable                      */
	#define RV32GC_MSTATUS_SD   (0x1 << 31) /**< State Dirty                          */
	/**@}*/

	/**
	 * @name Machine Cause Register (mcause)
	 */
	/**@{*/
	#define RV32GC_MCAUSE_INT   0x80000000 /**< Interrupt? */
	#define RV32GC_MCAUSE_CAUSE 0x0000000f /**< Trap Cause */
	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32GC_MCAUSE_USI  0 /**< User Software Interrupt        */
	#define RV32GC_MCAUSE_SSI  1 /**< Supervisor Software Interrupt  */
	#define RV32GC_MCAUSE_MSI  3 /**< Machine Software Interrupt     */
	#define RV32GC_MCAUSE_UTI  4 /**< User Timer Interrupt           */
	#define RV32GC_MCAUSE_STI  5 /**< Supervisor Timer Interrupt     */
	#define RV32GC_MCAUSE_MTI  7 /**< Machine Timer Interrupt        */
	#define RV32GC_MCAUSE_UEI  8 /**< User External Interrupt        */
	#define RV32GC_MCAUSE_SEI  9 /**< Supervisor External Interrupt  */
	#define RV32GC_MCAUSE_MEI 11 /**< Machine External Interrupt     */
	/**@}*/

	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32GC_MCAUSE_ALIGN_I      0 /**< Instruction Address Misaligned */
	#define RV32GC_MCAUSE_PG_PROT_I    1 /**< Instruction Access Fault       */
	#define RV32GC_MCAUSE_ILLEGAL_I    2 /**< Illegal Instruction            */
	#define RV32GC_MCAUSE_BREAK        3 /**< Breakpoint                     */
	#define RV32GC_MCAUSE_ALIGN_L      4 /**< Load Address Misaligned        */
	#define RV32GC_MCAUSE_PG_PROT_L    5 /**< Load Access Fault              */
	#define RV32GC_MCAUSE_ALIGN_S      6 /**< Store/AMO Address Misaligned   */
	#define RV32GC_MCAUSE_PG_PROT_S    7 /**< Store/AMO Access Fault         */
	#define RV32GC_MCAUSE_ECALL_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32GC_MCAUSE_ECALL_SMODE  9 /**< Environment Call from S-mode   */
	#define RV32GC_MCAUSE_ECALL_MMODE 11 /**< Environment Call from M-mode   */
	#define RV32GC_MCAUSE_PG_FAULT_I  12 /**< Instruction Page Fault         */
	#define RV32GC_MCAUSE_PG_FAULT_L  13 /**< Load Page Fault                */
	#define RV32GC_MCAUSE_PG_FAULT_S  15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Machine Interrupt Enable Register (mie)
	 */
	/**@{*/
	#define RV32GC_MIE_USIE (0x1 <<  0) /**< User Software Interrupt Enable       */
	#define RV32GC_MIE_SSIE (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32GC_MIE_MSIE (0x1 <<  3) /**< Machine Software Interrupt Enable    */
	#define RV32GC_MIE_UTIE (0x1 <<  4) /**< User Timer Interrupt Enable          */
	#define RV32GC_MIE_STIE (0x1 <<  5) /**< Supervisor Timer Interrupt Enable    */
	#define RV32GC_MIE_MTIE (0x1 <<  7) /**< Machine Timer Interrupt Enable       */
	#define RV32GC_MIE_UEIE (0x1 <<  8) /**< User External Interrupt Enable       */
	#define RV32GC_MIE_SEIE (0x1 <<  9) /**< Supervisor External Interrupt Enable */
	#define RV32GC_MIE_MEIE (0x1 << 11) /**< Machine External Interrupt Enable    */
	/**@}*/

	/**
	 * @name Machine Interrupt Pending Register (mip)
	 */
	/**@{*/
	#define RV32GC_MIP_USIP (0x1 <<  0) /**< User Software Interrupt Pending       */
	#define RV32GC_MIP_SSIP (0x1 <<  1) /**< Supervisor Interrupt Pending          */
	#define RV32GC_MIP_MSIP (0x1 <<  3) /**< Machine Software Interrupt Pending    */
	#define RV32GC_MIP_UTIP (0x1 <<  4) /**< User Timer Interrupt Pending          */
	#define RV32GC_MIP_STIP (0x1 <<  5) /**< Supervisor Timer Interrupt Pending    */
	#define RV32GC_MIP_MTIP (0x1 <<  7) /**< Machine Timer Interrupt Pending       */
	#define RV32GC_MIP_UEIP (0x1 <<  8) /**< User External Interrupt Pending       */
	#define RV32GC_MIP_SEIP (0x1 <<  9) /**< Supervisor External Interrupt Pending */
	#define RV32GC_MIP_MEIP (0x1 << 11) /**< Machine External Interrupt Pending    */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Reads a CSR register.
	 *
	 * @param regname Name of target CSR register.
	 *
	 * @returns The value of the @p csr register.
	 *
	 * @note Thinks works thanks to GCC extensions.
	 */
	#define rv32gc_csr_read(regname) \
		__extension__({             \
		 rv32gc_word_t __tmp;        \
		  __asm__ __volatile__ (    \
			"csrr %0, " #regname    \
			: "=r"(__tmp));         \
		    __tmp;                  \
		})

	/**
	 * @brief Sets a bit in the CSR register.
	 *
	 * @param regname Name of target CSR register.
	 * @param bit     Target bit in the CSR register.
	 *
	 * @note Thinks works thanks to GCC extensions.
	 */
	#define rv32gc_csr_set(regname, bit)  \
		__extension__({                  \
		 rv32gc_word_t __tmp;             \
		  __asm__ __volatile__ (         \
			"csrrs %0, " #regname ", %1" \
			: "=r"  (__tmp)              \
			:  "rK" (bit));              \
			__tmp;                       \
		})

	/**
	 * @brief Clears a bit in the CSR register.
	 *
	 * @param regname Name of target CSR register.
	 * @param bit     Target bit in the CSR register.
	 *
	 * @note Thinks works thanks to GCC extensions.
	 */
	#define rv32gc_csr_clear(regname, bit) \
		__extension__({                   \
		 rv32gc_word_t __tmp;              \
		  __asm__ __volatile__ (          \
			"csrrc %0, " #regname ", %1"  \
			: "=r"  (__tmp)               \
			:  "rK" (bit));               \
			__tmp;                        \
		})

	/**
	 * @brief Reads the misa register.
	 *
	 * @returns The value of the misa register.
	 */
	static inline rv32gc_word_t rv32gc_misa_read(void)
	{
		rv32gc_word_t misa;

		__asm__ __volatile__(
			"csrr %0, misa"
			: "=r" (misa)
		);

		return (misa);
	}

	/**
	 * @brief Reads the mhartid register.
	 *
	 * @returns The value of the mhartid register.
	 */
	static inline rv32gc_word_t rv32gc_mhartid_read(void)
	{
		rv32gc_word_t mhartid;

		__asm__ __volatile__(
			"csrr %0, mhartid"
			: "=r" (mhartid)
		);

		return (mhartid);
	}

	/**
	 * @brief Reads the mstatus register.
	 *
	 * @returns The value of the mstatus register.
	 */
	static inline rv32gc_word_t rv32gc_mstatus_read(void)
	{
		rv32gc_word_t mstatus;

		__asm__ __volatile__(
			"csrr %0, mstatus"
			: "=r" (mstatus)
		);

		return (mstatus);
	}

	/**
	 * @brief Writes to the mstatus register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mstatus_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mstatus, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mie register.
	 *
	 * @returns The value of the mie register.
	 */
	static inline rv32gc_word_t rv32gc_mie_read(void)
	{
		rv32gc_word_t mie;

		__asm__ __volatile__(
			"csrr %0, mie"
			: "=r" (mie)
		);

		return (mie);
	}

	/**
	 * @brief Writes to the mie register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mie_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mie, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mip register.
	 *
	 * @returns The value of the mip register.
	 */
	static inline rv32gc_word_t rv32gc_mip_read(void)
	{
		rv32gc_word_t mip;

		__asm__ __volatile__(
			"csrr %0, mip"
			: "=r" (mip)
		);

		return (mip);
	}

	/**
	 * @brief Writes to the mip register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mip_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mip, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mideleg register.
	 *
	 * @returns The value of the mideleg register.
	 */
	static inline rv32gc_word_t rv32gc_mideleg_read(void)
	{
		rv32gc_word_t mideleg;

		__asm__ __volatile__(
			"csrr %0, mideleg"
			: "=r" (mideleg)
		);

		return (mideleg);
	}

	/**
	 * @brief Writes to the mideleg register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mideleg_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mideleg, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mtvec register.
	 *
	 * @returns The value of the mtvec register.
	 */
	static inline rv32gc_word_t rv32gc_mtvec_read(void)
	{
		rv32gc_word_t mtvec;

		__asm__ __volatile__(
			"csrr %0, mtvec"
			: "=r" (mtvec)
		);

		return (mtvec);
	}

	/**
	 * @brief Writes to the mtvec register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mtvec_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mtvec, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mepc register.
	 *
	 * @returns The value of the mepc register.
	 */
	static inline rv32gc_word_t rv32gc_mepc_read(void)
	{
		rv32gc_word_t mepc;

		__asm__ __volatile__(
			"csrr %0, mepc"
			: "=r" (mepc)
		);

		return (mepc);
	}

	/**
	 * @brief Writes to the mepc register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_mepc_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw mepc, %0;"
			:
			: "r" (val)
		);
	}

	/**
	 * @brief Reads the mcause register.
	 *
	 * @returns The value of the mcause register.
	 */
	static inline rv32gc_word_t rv32gc_mcause_read(void)
	{
		rv32gc_word_t mcause;

		__asm__ __volatile__(
			"csrr %0, mcause"
			: "=r" (mcause)
		);

		return (mcause);
	}

	/**
	 * @brief Reads the mtval register.
	 *
	 * @returns The value of the mtval register.
	 */
	static inline rv32gc_word_t rv32gc_mtval_read(void)
	{
		rv32gc_word_t mtval;

		__asm__ __volatile__(
			"csrr %0, mtval"
			: "=r" (mtval)
		);

		return (mtval);
	}

	/**
	 * @brief Reads the medeleg register.
	 *
	 * @returns The value of the medeleg register.
	 */
	static inline rv32gc_word_t rv32gc_medeleg_read(void)
	{
		rv32gc_word_t medeleg;

		__asm__ __volatile__(
			"csrr %0, medeleg"
			: "=r" (medeleg)
		);

		return (medeleg);
	}

	/**
	 * @brief Writes to the medeleg register.
	 *
	 * @param val Value to write.
	 */
	static inline void rv32gc_medeleg_write(rv32gc_word_t val)
	{
		__asm__ __volatile__(
			"csrw medeleg, %0;"
			:
			: "r" (val)
		);
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32GC_MREGS_H_ */
