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
#define __NEED_CORE_TYPES

#include <arch/core/rv32i/core.h>
#include <arch/core/rv32i/clock.h>
#include <arch/core/rv32i/excp.h>
#include <arch/core/rv32i/int.h>
#include <arch/core/rv32i/types.h>
#include <arch/stdout/16550a.h>
#include <nanvix/const.h>
#include <errno.h>

#define UNREACHABLE() \
	{ while(TRUE) ; __builtin_unreachable(); }

/**
 * Current privilege level.
 */
PUBLIC int rv32i_curr_prv = RV32I_PRV_M;

/**
 * @brief Raises privilege level.
 *
 * The rv32i_prv_raise() raises the current privilege level to machine
 * mode.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_prv_raise(void)
{
	rv32i_curr_prv = RV32I_PRV_M;
}

/**
 * @brief Drops privilege level.
 *
 * The rv32i_prv_raise() drops the current privilege level to the
 * previous privilege level, which is stored in the mpp field of the
 * mstatus register.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_prv_drop(void)
{
	rv32i_word_t mstatus;

	mstatus = rv32i_mstatus_read();
	rv32i_curr_prv = BITS_GET(mstatus, RV32I_MSTATUS_MPP);
}

/**
 * @brief Issues machine-mode return.
 */
NORETURN static inline void rv32i_mret(void)
{
	__asm__ __volatile__ ("mret");

	UNREACHABLE();
}

/**
 * @brief Enters supervisor mode.
 *
 * @param pc Target program counter.
 */
PRIVATE NORETURN void rv32i_supervisor_enter(rv32i_word_t pc)
{
	rv32i_word_t mstatus;

	kprintf("[hal] entering supervisor mode...");

	/*
	 * Set supervisor priviledge mode
	 * and disable interrupts.
	 */
	mstatus = rv32i_mstatus_read();
	mstatus = BITS_SET(mstatus, RV32I_MSTATUS_MPP, RV32I_PRV_S);
	mstatus = BITS_SET(mstatus, RV32I_MSTATUS_MPIE, 0);
	rv32i_mstatus_write(mstatus);

	/* Set target program counter. */
	rv32i_prv_drop();
	rv32i_mepc_write(pc);

	rv32i_mret();
}

/**
 * @brief Handles a bad machine exception.
 */
PUBLIC NORETURN void rv32i_do_mbad(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] bad exception");
	UNREACHABLE();
}

/**
 * @brief Handles machine exceptions.
 *
 * @param ctx Interrupted context
 */
PUBLIC NORETURN void rv32i_do_mexcp(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] unhandled exception");
	UNREACHABLE();
}

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
 * @brief Handles machine calls.
 *
 * @param ctx Interrupted context
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
 * @brief Handles machine interrupts.
 *
 * @param ctx Interrupted context.
 */
PUBLIC void rv32i_do_mint(const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);
	rv32i_dump_all_csr();

	kprintf("[rv32i] unhandled interrupt");
	UNREACHABLE();
}

/**
 * @brief Delegates traps to loower-privilege levels.
 *
 * @todo: XXX check if supervisor mode is supported.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void rv32i_machine_delegate_traps(void)
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

/**
 * @brief Initializes machine mode. 
 *
 * @param pc Target program counter.
 */
PUBLIC NORETURN void rv32i_machine_setup(rv32i_word_t pc)
{
	rv32i_word_t mie;

	/*
	 * Early initialization of
	 * UART to help us debugging.
	 */
	uart_16550a_init();

	/* Enable machine IRQs. */
	mie = rv32i_mie_read();
	mie = BITS_SET(mie, RV32I_MIE_MSIE, 1);
	mie = BITS_SET(mie, RV32I_MIE_MTIE, 1);
	mie = BITS_SET(mie, RV32I_MIE_MEIE, 1);
	rv32i_mie_write(mie);

	rv32i_machine_delegate_traps();

	rv32i_supervisor_enter(pc);
}
