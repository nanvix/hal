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

/* Must come first. */
#define __NEED_CORE_CONTEXT
#define __NEED_CORE_TYPES

#include <arch/core/rv32i/context.h>
#include <arch/core/rv32i/excp.h>
#include <arch/core/rv32i/types.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * @brief Reads a CSR register.
 *
 * @param csr Target CSR register.
 *
 * @returns The value of the @p csr register.
 */
PRIVATE rv32i_word_t rv32i_do_csr_read(rv32i_word_t csr)
{
	switch(csr)
	{
		case RV32I_CSR_MHARTID:
			return (rv32i_csr_read(mhartid));

		default:
			kprintf("[rv32i] unknown csr to read");
			UNREACHABLE();
	}

	return (0);
}

/**
 * @brief Sets a bit in a CSR register.
 *
 * @param csr Target CSR register.
 * @param bit Target bit in the CSR register.
 *
 * @returns Previous CSR register value.
 */
PRIVATE rv32i_word_t rv32i_do_csr_set(rv32i_word_t csr, rv32i_word_t bit)
{
	switch(csr)
	{
		case RV32I_CSR_MSTATUS:
			return (rv32i_csr_set(mstatus, bit));

		case RV32I_CSR_MIE:
			return (rv32i_csr_set(mie, bit));

		default:
			kprintf("[rv32i] unknown csr to set");
			UNREACHABLE();
	}

	return (0);
}

/**
 * @brief Clears a bit in a CSR register.
 *
 * @param csr Target CSR register.
 * @param bit Target bit in the CSR register.
 *
 * @returns Previous CSR register value.
 */
PRIVATE rv32i_word_t rv32i_do_csr_clear(rv32i_word_t csr, rv32i_word_t bit)
{
	switch(csr)
	{
		case RV32I_CSR_MSTATUS:
			return (rv32i_csr_clear(mstatus, bit));

		case RV32I_CSR_MIE:
			return (rv32i_csr_clear(mie, bit));

		default:
			kprintf("[rv32i] unknown csr to clear");
			UNREACHABLE();
	}

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_dump_all_csr(void)
{
	kprintf("[hal]  mstatus=%x      mie=%x     mip=%x",
		rv32i_mstatus_read(),
		rv32i_mie_read(),
		rv32i_mip_read()
	);
	kprintf("[hal]  sstatus=%x      sie=%x     sip=%x",
		rv32i_sstatus_read(),
		rv32i_sie_read(),
		rv32i_sip_read()
	);
	kprintf("[hal]    mtvec=%x   mcause=%x   mtval=%x",
	rv32i_mhartid_read(),
		rv32i_mcause_read(),
		rv32i_mtval_read()
	);
	kprintf("[hal]    stvec=%x   scause=%x   stval=%x",
		rv32i_stvec_read(),
		rv32i_scause_read(),
		rv32i_stval_read()
	);
	kprintf("[hal]     mepc=%x",
		rv32i_mepc_read()
	);
	kprintf("[hal]     sepc=%x    satp=%x",
		rv32i_sepc_read(),
		rv32i_satp_read()
	);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32i_supervisor_enter(rv32i_word_t pc)
{
	rv32i_word_t mstatus;

	kprintf("[hal] entering supervisor mode...");

	/*
	 * Set supervisor privilege mode
	 * and disable interrupts.
	 */
	mstatus = rv32i_mstatus_read();
	mstatus = BITS_SET(mstatus, RV32I_MSTATUS_MPP, RV32I_PRV_S);
	mstatus = BITS_SET(mstatus, RV32I_MSTATUS_MPIE, 0);
	rv32i_mstatus_write(mstatus);

	/* Set target program counter. */
	rv32i_mepc_write(pc);

	asm volatile ("mret");

	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32i_do_mbad(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] bad exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32i_do_mexcp(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] unhandled exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_do_mcall(struct context *ctx)
{
	rv32i_word_t mstatus;

	/* Parse machine call. */
	switch (ctx->a0)
	{
		/* Read CSR register. */
		case RV32I_MCALL_CSR_READ:
			ctx->a0 = rv32i_do_csr_read(ctx->a1);
			break;

		/* Set bit in CSR register. */
		case RV32I_MCALL_CSR_SET:
			ctx->a0 = rv32i_do_csr_set(ctx->a1, ctx->a2);
			break;

		/* Clear bit in CSR register. */
		case RV32I_MCALL_CSR_CLEAR:
			ctx->a0 = rv32i_do_csr_clear(ctx->a1, ctx->a2);
			break;

		/* Enable machine interrupts. */
		case RV32I_MCALL_MINT_ENABLE:
			mstatus = rv32i_mstatus_read();
			mstatus |= RV32I_MSTATUS_MPIE;
			rv32i_mstatus_write(mstatus);
			ctx->a0 = 0;
			break;

		/* Disable machine interrupts. */
		case RV32I_MCALL_MINT_DISABLE:
			mstatus = rv32i_mstatus_read();
			mstatus &= ~RV32I_MSTATUS_MPIE;
			rv32i_mstatus_write(mstatus);
			ctx->a0 = 0;
			break;

		/* Write mtimecmp register. */
		case RV32I_MCALL_TIMER_ACK:
			rv32i_csr_set(mie, RV32I_MIE_MTIE);
			ctx->a0 = 0;
		break;

		default:
			kprintf("[rv32i] unsupported machine call");
			ctx->a0 = -ENOTSUP;
	}

	/*
	 * Advance stack pointer, otherwise
	 * we will end up in an infinite loop.
	 */
	ctx->pc += RV32I_WORD_SIZE;
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_do_mint(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] unhandled interrupt");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_machine_delegate_traps(void)
{
	rv32i_word_t medeleg;
	rv32i_word_t mideleg;

	mideleg =
		RV32I_MIP_SSIP | RV32I_MIP_STIP | RV32I_MIP_SEIP;

	medeleg =
		(1 << RV32I_MCAUSE_ALIGN_I)     |
		(1 << RV32I_MCAUSE_BREAK)       |
		(1 << RV32I_MCAUSE_ECALL_UMODE) |
		(1 << RV32I_MCAUSE_PG_FAULT_I)  |
		(1 << RV32I_MCAUSE_PG_FAULT_L)  |
		(1 << RV32I_MCAUSE_PG_FAULT_S);

	rv32i_medeleg_write(medeleg);
	rv32i_mideleg_write(mideleg);

	/* XXX: ensure that these values were written. */
}
