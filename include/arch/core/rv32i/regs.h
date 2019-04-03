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

#ifndef ARCH_CORE_RV32I_REGS_H_
#define ARCH_CORE_RV32I_REGS_H_

	#ifndef __NEED_CORE_REGS
		#error "do not include this file"
	#endif

/**
 * @addtogroup rv32i-core-regs Registers
 * @ingroup rv32i-core
 *
 * @brief Registers
 */
/**@{*/

	#define __NEED_CORE_TYPES
	#include <arch/core/rv32i/types.h>

/*============================================================================*
 * Machine CSRs                                                               *
 *============================================================================*/

	/**
	 * @name Machine Information Registers
	 */
	/**@{*/
	#define RV32I_CSR_MVENDORID 0xf11 /**< Machine Vendor ID         */
	#define RV32I_CSR_MARCHID   0xf12 /**< Marchine Architecture ID  */
	#define RV32I_CSR_MIMPID    0xf13 /**< Machine ID Implementation */
	#define RV32I_CSR_MHARTID   0xf14 /**< Machine Hart ID           */
	/**@}*/

	/**
	 * @name Machine Trap Setup Registers
	 */
	/**@{*/
	#define RV32I_CSR_MSTATUS    0x300 /**< Machine Status               */
	#define RV32I_CSR_MISA       0x301 /**< Machine ISA and Extensions   */
	#define RV32I_CSR_MEDELEG    0x302 /**< Machine Exception Delegation */
	#define RV32I_CSR_MIDELEG    0x303 /**< Machine Interrupt Delegation */
	#define RV32I_CSR_MIE        0x304 /**< Machine Interrupt Enable     */
	#define RV32I_CSR_MTVEC      0x305 /**< Machine Trap Vector          */
	#define RV32I_CSR_MCOUNTEREN 0x306 /**< Machine Counter-Enable       */
	/**@}*/

	/**
	 * @name Machine Trap Handling Registers
	 */
	/**@{*/
	#define RV32I_CSR_MSCRATCH 0x340 /**< Machine Scratch                   */
	#define RV32I_CSR_MEPC     0x341 /**< Machine Exception Program Counter */
	#define RV32I_CSR_MCAUSE   0x342 /**< Machine Exception Cause           */
	#define RV32I_CSR_MTVAL    0x343 /**< Machine Trap Value                */
	#define RV32I_CSR_MIP      0x344 /**< Machine Interrupt Pending         */
	/**@}*/

	/**
	 * @name Privilege Modes
	 */
	/**@{*/
	#define RV32I_PRV_U 0 /**< User/Application */
	#define RV32I_PRV_S 1 /**< Supervisor       */
	#define RV32I_PRV_M 3 /**< Machine          */
	/**@}*/

	/**
	 * @name Machine Status Register (mstatus)
	 */
	/**@{*/
	#define RV32I_MSTATUS_UIE  (0x1 <<  0) /**< User Interrupt Enable                */
	#define RV32I_MSTATUS_SIE  (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_MSTATUS_MIE  (0x1 <<  3) /**< Machine Interrupt Enable             */
	#define RV32I_MSTATUS_UPIE (0x1 <<  4) /**< User Previous Interrupt Enable       */
	#define RV32I_MSTATUS_SPIE (0x1 <<  5) /**< Supervisor Previous Interrupt Enable */
	#define RV32I_MSTATUS_MPIE (0x1 <<  7) /**< Machine Previous Interrupt Enable    */
	#define RV32I_MSTATUS_SPP  (0x1 <<  8) /**< Supervisor Previous Priviledge       */
	#define RV32I_MSTATUS_MPP  (0x3 << 11) /**< Machine Previous Priviledge          */
	#define RV32I_MSTATUS_SUM  (0x1 << 18) /**< Allow Supervisor User Memory Access  */
	#define RV32I_MSTATUS_MXR  (0x1 << 19) /**< Make Executable                      */
	#define RV32I_MSTATUS_SD   (0x1 << 31) /**< State Dirty                          */
	/**@}*/

	/**
	 * @name Machine Cause Register (mcause)
	 */
	/**@{*/
	#define RV32I_MCAUSE_INT   0x80000000 /**< Interrupt? */
	#define RV32I_MCAUSE_CAUSE 0x0000000f /**< Trap Cause */
	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32I_MCAUSE_USI  0 /**< User Software Interrupt        */
	#define RV32I_MCAUSE_SSI  1 /**< Supervisor Software Interrupt  */
	#define RV32I_MCAUSE_MSI  3 /**< Machine Software Interrupt     */
	#define RV32I_MCAUSE_UTI  4 /**< User Timer Interrupt           */
	#define RV32I_MCAUSE_STI  5 /**< Supervisor Timer Interrupt     */
	#define RV32I_MCAUSE_MTI  7 /**< Machine Timer Interrupt        */
	#define RV32I_MCAUSE_UEI  8 /**< User External Interrupt        */
	#define RV32I_MCAUSE_SEI  9 /**< Supervisor External Interrupt  */
	#define RV32I_MCAUSE_MEI 11 /**< Machine External Interrupt     */
	/**@}*/

	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32I_MCAUSE_ALIGN_I      0 /**< Instruction Address Misaligned */
	#define RV32I_MCAUSE_PG_PROT_I    1 /**< Instruction Access Fault       */
	#define RV32I_MCAUSE_ILLEGAL_I    2 /**< Illegal Instruction            */
	#define RV32I_MCAUSE_BREAK        3 /**< Breakpoint                     */
	#define RV32I_MCAUSE_ALIGN_L      4 /**< Load Address Misaligned        */
	#define RV32I_MCAUSE_PG_PROT_L    5 /**< Load Access Fault              */
	#define RV32I_MCAUSE_ALIGN_S      6 /**< Store/AMO Address Misaligned   */
	#define RV32I_MCAUSE_PG_PROT_S    7 /**< Store/AMO Access Fault         */
	#define RV32I_MCAUSE_ECALL_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32I_MCAUSE_ECALL_SMODE  9 /**< Environment Call from S-mode   */
	#define RV32I_MCAUSE_ECALL_MMODE 11 /**< Environment Call from M-mode   */
	#define RV32I_MCAUSE_PG_FAULT_I  12 /**< Instruction Page Fault         */
	#define RV32I_MCAUSE_PG_FAULT_L  13 /**< Load Page Fault                */
	#define RV32I_MCAUSE_PG_FAULT_S  15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Machine Interrupt Enable Register (mie)
	 */
	/**@{*/
	#define RV32I_MIE_USIE (0x1 <<  0) /**< User Software Interrupt Enable       */
	#define RV32I_MIE_SSIE (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_MIE_MSIE (0x1 <<  3) /**< Machine Software Interrupt Enable    */
	#define RV32I_MIE_UTIE (0x1 <<  4) /**< User Timer Interrupt Enable          */
	#define RV32I_MIE_STIE (0x1 <<  5) /**< Supervisor Timer Interrupt Enable    */
	#define RV32I_MIE_MTIE (0x1 <<  7) /**< Machine Timer Interrupt Enable       */
	#define RV32I_MIE_UEIE (0x1 <<  8) /**< User External Interrupt Enable       */
	#define RV32I_MIE_SEIE (0x1 <<  9) /**< Supervisor External Interrupt Enable */
	#define RV32I_MIE_MEIE (0x1 << 11) /**< Machine External Interrupt Enable    */
	/**@}*/

	/**
	 * @name Machine Interrupt Pending Register (mip)
	 */
	/**@{*/
	#define RV32I_MIP_USIP (0x1 <<  0) /**< User Software Interrupt Pending       */
	#define RV32I_MIP_SSIP (0x1 <<  1) /**< Supervisor Interrupt Pending          */
	#define RV32I_MIP_MSIP (0x1 <<  3) /**< Machine Software Interrupt Pending    */
	#define RV32I_MIP_UTIP (0x1 <<  4) /**< User Timer Interrupt Pending          */
	#define RV32I_MIP_STIP (0x1 <<  5) /**< Supervisor Timer Interrupt Pending    */
	#define RV32I_MIP_MTIP (0x1 <<  7) /**< Machine Timer Interrupt Pending       */
	#define RV32I_MIP_UEIP (0x1 <<  8) /**< User External Interrupt Pending       */
	#define RV32I_MIP_SEIP (0x1 <<  9) /**< Supervisor External Interrupt Pending */
	#define RV32I_MIP_MEIP (0x1 << 11) /**< Machine External Interrupt Pending    */
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
	#define rv32i_csr_read(regname) \
		__extension__({             \
		 rv32i_word_t __tmp;        \
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
	#define rv32i_csr_set(regname, bit)  \
		__extension__({                  \
		 rv32i_word_t __tmp;             \
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
	#define rv32i_csr_clear(regname, bit) \
		__extension__({                   \
		 rv32i_word_t __tmp;              \
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
	static inline rv32i_word_t rv32i_misa_read(void)
	{
		rv32i_word_t misa;

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
	static inline rv32i_word_t rv32i_mhartid_read(void)
	{
		rv32i_word_t mhartid;

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
	static inline rv32i_word_t rv32i_mstatus_read(void)
	{
		rv32i_word_t mstatus;

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
	static inline void rv32i_mstatus_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mie_read(void)
	{
		rv32i_word_t mie;

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
	static inline void rv32i_mie_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mip_read(void)
	{
		rv32i_word_t mip;

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
	static inline void rv32i_mip_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mideleg_read(void)
	{
		rv32i_word_t mideleg;

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
	static inline void rv32i_mideleg_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mtvec_read(void)
	{
		rv32i_word_t mtvec;

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
	static inline void rv32i_mtvec_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mepc_read(void)
	{
		rv32i_word_t mepc;

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
	static inline void rv32i_mepc_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_mcause_read(void)
	{
		rv32i_word_t mcause;

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
	static inline rv32i_word_t rv32i_mtval_read(void)
	{
		rv32i_word_t mtval;

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
	static inline rv32i_word_t rv32i_medeleg_read(void)
	{
		rv32i_word_t medeleg;

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
	static inline void rv32i_medeleg_write(rv32i_word_t val)
	{
		__asm__ __volatile__(
			"csrw medeleg, %0;"
			:
			: "r" (val)
		);
	}

#endif /* _ASM_FILE_ */

/*============================================================================*
 * Supervisor CSRs                                                            *
 *============================================================================*/

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
	#define RV32I_CSR_SSTATUS    0x100 /**< Supervisor Status           */
	#define RV32I_CSR_SIE        0x104 /**< Supervisor Interrupt Enable */
	#define RV32I_CSR_STVEC      0x105 /**< Supervisor Trap Vector      */
	#define RV32I_CSR_SCOUNTEREN 0x106 /**< Supervisor Counter-Enable   */
	/**@}*/

	/**
	 * @brief Supervisor Trap Handling Registes
	 */
	/**@{*/
	#define RV32I_CSR_SSCRATCH 0x140 /**< Supervisor Scratch                   */
	#define RV32I_CSR_SEPC     0x141 /**< Supervisor Exception Program Counter */
	#define RV32I_CSR_SCAUSE   0x142 /**< Supervisor Exception Cause           */
	#define RV32I_CSR_STVAL    0x143 /**< Supervisor Trap Value                */
	#define RV32I_CSR_SIP      0x144 /**< Supervisor Interrupt Pending         */
	/**@}*/

	/**
	 * @name Supervisor Protection and Translation Registers
	 */
	/**@{*/
	#define RV32I_CSR_SATP 0x180 /**< Supervisor Address Translation and Protection */
	/**@}*/

	/**
	 * @name Supervisor Status Register (sstatus)
	 */
	/**@{*/
	#define RV32I_SSTATUS_UIE   (0x1 <<  0) /**< User Interrupt Enable                */
	#define RV32I_SSTATUS_SIE   (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_SSTATUS_UPIE  (0x1 <<  4) /**< User Previous Interrupt Enable       */
	#define RV32I_SSTATUS_SPIE  (0x1 <<  5) /**< Supervisor Previous Interrupt Enable */
	#define RV32I_SSTATUS_SPP   (0x1 <<  8) /**< Supervisor Previous Priviledge       */
	#define RV32I_SSTATUS_SUM   (0x1 << 18) /**< Allow Supervisor User Memory Access  */
	#define RV32I_SSTATUS_MXR   (0x1 << 19) /**< Make Executable                      */
	#define RV32I_SSTATUS_SD    (0x1 << 31) /**< State Dirty                          */
	/**@}*/

	/**
	 * @name Supervisor Interrupt Enable Register (sie)
	 */
	/**@{*/
	#define RV32I_SIE_USIE (0x1 <<  0) /**< User Software Interrupt Enable       */
	#define RV32I_SIE_SSIE (0x1 <<  1) /**< Supervisor Interrupt Enable          */
	#define RV32I_SIE_UTIE (0x1 <<  4) /**< User Timer Interrupt Enable          */
	#define RV32I_SIE_STIE (0x1 <<  5) /**< Supervisor Timer Interrupt Enable    */
	#define RV32I_SIE_UEUE (0x1 <<  8) /**< User External Interrupt Enable       */
	#define RV32I_SIE_SEIE (0x1 <<  9) /**< Supervisor External Interrupt Enable */
	/**@}*/

	/**
	 * @name Supervisor Interrupt Pending (sip)
	 */
	/**@{*/
	#define RV32I_SIP_USIP (0x1 << 0) /**< User Software Interrupt Pending       */
	#define RV32I_SIP_SSIP (0x1 << 1) /**< Supervisor Software Interrupt Pending */
	#define RV32I_SIP_UTIP (0x1 << 4) /**< User Timer Interrupt Pending          */
	#define RV32I_SIP_STIP (0x1 << 5) /**< Supervisor Timer Interrupt Pending    */
	#define RV32I_SIP_UEIP (0x1 << 8) /**< User External Interrupt Pending       */
	#define RV32I_SIP_SEIP (0x1 << 9) /**< Supervisor External Interrupt Pending */
	/**@}*/

	/**
	 * @name Machine Cause Register (mcause)
	 */
	/**@{*/
	#define RV32I_SCAUSE_INT   0x80000000 /**< Interrupt? */
	#define RV32I_SCAUSE_CAUSE 0x0000000f /**< Trap Cause */
	/**@}*/

	/**
	 * @name Exception Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32I_SCAUSE_ALIGN_I      0 /**< Instruction Address Misaligned */
	#define RV32I_SCAUSE_PG_PROT_I    1 /**< Instruction Access Fault       */
	#define RV32I_SCAUSE_ILLEGAL_I    2 /**< Illegal Instruction            */
	#define RV32I_SCAUSE_BREAK        3 /**< Breakpoint                     */
	#define RV32I_SCAUSE_PG_PROT_L    5 /**< Load Access Fault              */
	#define RV32I_SCAUSE_ALIGN_S      6 /**< Store/AMO Address Misaligned   */
	#define RV32I_SCAUSE_PG_PROT_S    7 /**< Store/AMO Access Fault         */
	#define RV32I_SCAUSE_ECALL_UMODE  8 /**< Environment Call from U-mode   */
	#define RV32I_SCAUSE_PG_FAULT_I  12 /**< Instruction Page Fault         */
	#define RV32I_SCAUSE_PG_FAULT_L  13 /**< Load Page Fault                */
	#define RV32I_SCAUSE_PG_FAULT_S  15 /**< Store/AMO Page Fault           */
	/**@}*/

	/**
	 * @name Interrupt Cause Codes for Supervisor Cause Register (scause.cause)
	 */
	/**@{*/
	#define RV32I_SCAUSE_USI 0 /**< User Software Interrupt        */
	#define RV32I_SCAUSE_SSI 1 /**< Supervisor Software Interrupt  */
	#define RV32I_SCAUSE_UTI 4 /**< User Timer Interrupt           */
	#define RV32I_SCAUSE_STI 5 /**< Supervisor Timer Interrupt     */
	#define RV32I_SCAUSE_UEI 8 /**< User External Interrupt        */
	#define RV32I_SCAUSE_SEI 9 /**< Supervisor External Interrupt  */
	/**@}*/

#ifndef _ASM_FILE_

	/**
	 * @brief Reads the sstatus register.
	 *
	 * @returns The value of the sstatus register.
	 */
	static inline rv32i_word_t rv32i_sstatus_read(void)
	{
		rv32i_word_t sstatus;

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
	static inline void rv32i_sstatus_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_sie_read(void)
	{
		rv32i_word_t sie;

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
	static inline void rv32i_sie_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_sip_read(void)
	{
		rv32i_word_t sip;

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
	static inline void rv32i_sip_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_stvec_read(void)
	{
		rv32i_word_t stvec;

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
	static inline void rv32i_stvec_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_sepc_read(void)
	{
		rv32i_word_t sepc;

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
	static inline void rv32i_sepc_write(rv32i_word_t val)
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
	static inline rv32i_word_t rv32i_scause_read(void)
	{
		rv32i_word_t scause;

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
	static inline rv32i_word_t rv32i_stval_read(void)
	{
		rv32i_word_t stval;

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
	static inline rv32i_word_t rv32i_satp_read(void)
	{
		rv32i_word_t satp;

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
	static inline void rv32i_satp_write(rv32i_word_t val)
	{
		__asm__ __volatile__(
			"csrw satp, %0;"
			:
			: "r" (val)
		);
	}

#endif /* _ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_RV32I_REGS_H_ */
