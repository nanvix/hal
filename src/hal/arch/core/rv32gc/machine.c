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

#include <arch/core/rv32gc/ctx.h>
#include <arch/core/rv32gc/excp.h>
#include <arch/core/rv32gc/types.h>
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
PRIVATE rv32gc_word_t rv32gc_do_csr_read(rv32gc_word_t csr)
{
	switch(csr)
	{
		case RV32GC_CSR_MHARTID:
			return (rv32gc_csr_read(mhartid));

		default:
			kprintf("[rv32gc] unknown csr to read");
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
PRIVATE rv32gc_word_t rv32gc_do_csr_set(rv32gc_word_t csr, rv32gc_word_t bit)
{
	switch(csr)
	{
		case RV32GC_CSR_MSTATUS:
			return (rv32gc_csr_set(mstatus, bit));

		case RV32GC_CSR_MIE:
			return (rv32gc_csr_set(mie, bit));

		default:
			kprintf("[rv32gc] unknown csr to set");
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
PRIVATE rv32gc_word_t rv32gc_do_csr_clear(rv32gc_word_t csr, rv32gc_word_t bit)
{
	switch(csr)
	{
		case RV32GC_CSR_MSTATUS:
			return (rv32gc_csr_clear(mstatus, bit));

		case RV32GC_CSR_MIE:
			return (rv32gc_csr_clear(mie, bit));

		default:
			kprintf("[rv32gc] unknown csr to clear");
			UNREACHABLE();
	}

	return (0);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_dump_all_csr(void)
{
	kprintf("[hal]  mstatus=%x      mie=%x     mip=%x",
		rv32gc_mstatus_read(),
		rv32gc_mie_read(),
		rv32gc_mip_read()
	);
	kprintf("[hal]  sstatus=%x      sie=%x     sip=%x",
		rv32gc_sstatus_read(),
		rv32gc_sie_read(),
		rv32gc_sip_read()
	);
	kprintf("[hal]    mtvec=%x   mcause=%x   mtval=%x",
	rv32gc_mhartid_read(),
		rv32gc_mcause_read(),
		rv32gc_mtval_read()
	);
	kprintf("[hal]    stvec=%x   scause=%x   stval=%x",
		rv32gc_stvec_read(),
		rv32gc_scause_read(),
		rv32gc_stval_read()
	);
	kprintf("[hal]     mepc=%x mhartid=%x",
		rv32gc_mepc_read(),
		rv32gc_mhartid_read()
	);
	kprintf("[hal]     sepc=%x    satp=%x",
		rv32gc_sepc_read(),
		rv32gc_satp_read()
	);
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32gc_supervisor_enter(rv32gc_word_t pc)
{
	rv32gc_word_t mstatus;

	kprintf("[hal] entering supervisor mode...");

	/*
	 * Set supervisor privilege mode
	 * and disable interrupts.
	 */
	mstatus = rv32gc_mstatus_read();
	mstatus = BITS_SET(mstatus, RV32GC_MSTATUS_MPP, RV32GC_PRV_S);
	mstatus = BITS_SET(mstatus, RV32GC_MSTATUS_MPIE, 0);
	rv32gc_mstatus_write(mstatus);

	/* Set target program counter. */
	rv32gc_mepc_write(pc);

	asm volatile ("mret");

	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32gc_do_mbad(const struct context *ctx)
{
	rv32gc_context_dump(ctx);
	rv32gc_dump_all_csr();

	kprintf("[rv32gc] bad exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void rv32gc_do_mexcp(const struct context *ctx)
{
	rv32gc_context_dump(ctx);
	rv32gc_dump_all_csr();

	kprintf("[rv32gc] unhandled exception");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_do_mcall(struct context *ctx)
{
	rv32gc_word_t mstatus;

	/* Parse machine call. */
	switch (ctx->a0)
	{
		/* Read CSR register. */
		case RV32GC_MCALL_CSR_READ:
			ctx->a0 = rv32gc_do_csr_read(ctx->a1);
			break;

		/* Set bit in CSR register. */
		case RV32GC_MCALL_CSR_SET:
			ctx->a0 = rv32gc_do_csr_set(ctx->a1, ctx->a2);
			break;

		/* Clear bit in CSR register. */
		case RV32GC_MCALL_CSR_CLEAR:
			ctx->a0 = rv32gc_do_csr_clear(ctx->a1, ctx->a2);
			break;

		/* Enable machine interrupts. */
		case RV32GC_MCALL_MINT_ENABLE:
			mstatus = rv32gc_mstatus_read();
			mstatus |= RV32GC_MSTATUS_MPIE;
			rv32gc_mstatus_write(mstatus);
			ctx->a0 = 0;
			break;

		/* Disable machine interrupts. */
		case RV32GC_MCALL_MINT_DISABLE:
			mstatus = rv32gc_mstatus_read();
			mstatus &= ~RV32GC_MSTATUS_MPIE;
			rv32gc_mstatus_write(mstatus);
			ctx->a0 = 0;
			break;

		/* Re-Enable IPIs. */
		case RV32GC_MCALL_IPI_ACK:
			rv32gc_csr_clear(mip, RV32GC_MIP_MSIP);
			rv32gc_csr_set(mie, RV32GC_MIE_MSIE);
			ctx->a0 = 0;
		break;

		/* Write mtimecmp register. */
		case RV32GC_MCALL_TIMER_ACK:
			rv32gc_csr_set(mie, RV32GC_MIE_MTIE);
			ctx->a0 = 0;
		break;

		default:
			kprintf("[rv32gc] unsupported machine call");
			ctx->a0 = -ENOTSUP;
	}

	/*
	 * Advance stack pointer, otherwise
	 * we will end up in an infinite loop.
	 */
	ctx->pc += RV32GC_WORD_SIZE;
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_do_mint(const struct context *ctx)
{
	rv32gc_context_dump(ctx);
	rv32gc_dump_all_csr();

	kprintf("[rv32gc] unhandled interrupt");
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32gc_machine_delegate_traps(void)
{
	rv32gc_word_t medeleg;
	rv32gc_word_t mideleg;

	mideleg =
		RV32GC_MIP_SSIP | RV32GC_MIP_STIP | RV32GC_MIP_SEIP;

	medeleg =
		(1 << RV32GC_MCAUSE_ALIGN_I)     |
		(1 << RV32GC_MCAUSE_BREAK)       |
		(1 << RV32GC_MCAUSE_ECALL_UMODE) |
		(1 << RV32GC_MCAUSE_PG_FAULT_I)  |
		(1 << RV32GC_MCAUSE_PG_FAULT_L)  |
		(1 << RV32GC_MCAUSE_PG_FAULT_S);

	rv32gc_medeleg_write(medeleg);
	rv32gc_mideleg_write(mideleg);

	/* XXX: ensure that these values were written. */
}
