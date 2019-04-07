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

#include <arch/core/rv32i/core.h>
#include <arch/core/rv32i/excp.h>
#include <arch/core/rv32i/cache.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <errno.h>

/**
 * @brief Information about exceptions.
 *
 * Lookup table with information about exceptions.
 */
PRIVATE const struct
{
	int code;           /**< Code.          */
	const char *errmsg; /**< Error message. */
} exceptions[RV32I_EXCP_NUM_EXT] = {
	{ RV32I_EXCP_ALIGNMENT_INSTR,          "instruction address misaligned" },
	{ RV32I_EXCP_PAGE_PROTECTION_INSTR,    "instruction access fault"       },
	{ RV32I_EXCP_OPCODE,                   "illegal instruction"            },
	{ RV32I_EXCP_BREAKPOINT,               "breakpoint"                     },
	{ RV32I_EXCP_ALIGNMENT_LOAD,           "load address misaligned"        },
	{ RV32I_EXCP_PAGE_PROTECTION_LOAD,     "load access fault"              },
	{ RV32I_EXCP_ALIGNMEN_STORE,           "store/amo address misaligned"   },
	{ RV32I_EXCP_PAGE_PROTECTION_STORE,    "store/amo access fault"         },
	{ RV32I_EXCP_GENERAL_PROTECTION_UMODE, "environment call from u-mode"   },
	{ RV32I_EXCP_GENERAL_PROTECTION_SMODE, "environment call from s-mode"   },
	{ RV32I_EXCP_RESERVED2,                "reserved"                       },
	{ RV32I_EXCP_GENERAL_PROTECTION_MMODE, "environment call from m-mode"   },
	{ RV32I_EXCP_PAGE_FAULT_INSTR,         "instruction page fault"         },
	{ RV32I_EXCP_PAGE_FAULT_LOAD,          "load page fault"                },
	{ RV32I_EXCP_RESERVED1,                "reserved"                       },
	{ RV32I_VIRT_PAGE_FAULT,               "page fault"                     },
	{ RV32I_VIRT_PAGE_PROTECTION,          "page protection"                },
	{ RV32I_VIRT_GENERAL_PROTECTION,       "general protection"             },
};

/**
 * @brief Exception handlers.
 *
 * Lookup table with registered exception handlers.
 */
PRIVATE rv32i_exception_handler_fn rv32i_excp_handlers[RV32I_EXCP_NUM_EXT] = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL
};

/**
 * @todo: FIXME comment this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_dump_all_gpr(const struct context *ctx)
{
	kprintf("[hal]  ra=%x   sp=%x   fp=%x",        ctx->ra, ctx->sp, ctx->fp);
	kprintf("[hal]  gp=%x   tp=%x",                ctx->gp, ctx->tp);
	kprintf("[hal]  a0=%x   a1=%x  s2=%x  a3=%x", ctx->a0, ctx->a1,  ctx->a2,  ctx->a3);
	kprintf("[hal]  a4=%x   a5=%x  s6=%x  a7=%x", ctx->a4, ctx->a5,  ctx->a6,  ctx->a7);
	kprintf("[hal]  t0=%x   t1=%x  t2=%x",        ctx->t0, ctx->t1,  ctx->t2);
	kprintf("[hal]  t3=%x   t4=%x  t5=%x  t6=%x", ctx->t3, ctx->t4,  ctx->t5,  ctx->t6);
	kprintf("[hal]  s1=%x   s2=%x  s3=%x  s4=%x", ctx->s1, ctx->s2,  ctx->s3,  ctx->s4);
	kprintf("[hal]  s5=%x   s6=%x  s7=%x  s8=%x", ctx->s5, ctx->s6,  ctx->s7,  ctx->s8);
	kprintf("[hal]  s9=%x  s10=%x s11=%x",        ctx->s9, ctx->s10, ctx->s11);
}

/**
 * @brief Handles an unhandled exception.
 *
 * @brief excp Exception information.
 * @brief ctx  Saved execution context.
 *
 * The do_generic_excp() function handles the unhandled exception. It
 * dumps as much information as possible about the state of the
 * underlying core and then it panics the kernel.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void do_generic_excp(const struct exception *excp, const struct context *ctx)
{
	rv32i_dump_all_gpr(ctx);

	kpanic("unhandled %s exception at %x\n", exceptions[excp->id].errmsg, excp->addr);
}

/**
 * @brief High-level exception dispatcher.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted context.
 *
 * The rv32i_do_excp() function dispatches an  exception to a
 * previously registered exception handler. If no handler was
 * registered before to handle the exception that was triggered, the
 * default handler is called.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void rv32i_do_excp(const struct exception *excp, const struct context *ctx)
{
	/* Unknown exception. */
	if (excp->id >= (RV32I_EXCP_NUM_EXT))
		kpanic("unknown exception %x\n", excp->id);

	/* Unhandled exception. */
	if (rv32i_excp_handlers[excp->id] == NULL)
		do_generic_excp(excp, ctx);

	rv32i_excp_handlers[excp->id](excp, ctx);
}

/**
 * @todo Document this function.
 */
PUBLIC void forward_excp(int num, const struct exception *excp, const struct context *ctx)
{
	struct exception *_excp = (struct exception *)excp;

	_excp->id = num;

	rv32i_do_excp(_excp, ctx);
}

/**
 * The rv32i_excp_set_handler() function sets a handler function for
 * the exception @p num.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32i_excp_set_handler(int num, rv32i_exception_handler_fn handler)
{
	/* Invalid exception number. */
	if ((num < 0) || (num >= (RV32I_EXCP_NUM_EXT)))
	{
		kprintf("[hal] invalid exception number");
		return (-EINVAL);
	}

	/* Invalid handler. */
	if (handler == NULL)
	{
		kprintf("[hal] invalid exception handler");
		return (-EINVAL);
	}

	/* Bad exception number. */
	if (rv32i_excp_handlers[num] != NULL)
		return (-EBUSY);

	rv32i_excp_handlers[num] = handler;
	rv32i_cache_inval();

	return (0);
}

/**
 * The rv32i_excp_unset_handler() function unsets a handler function for
 * the exception @p num.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int rv32i_excp_unset_handler(int num)
{
	/* Invalid exception number. */
	if ((num < 0) || (num >= (RV32I_EXCP_NUM_EXT)))
	{
		kprintf("[hal] invalid exception number");
		return (-EINVAL);
	}

	/* Bad exception number. */
	if (rv32i_excp_handlers[num] == NULL)
		return (-EINVAL);

	rv32i_excp_handlers[num] = NULL;
	rv32i_cache_inval();

	return (0);
}
