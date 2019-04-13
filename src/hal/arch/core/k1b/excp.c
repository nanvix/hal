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

#include <arch/core/k1b/excp.h>
#include <arch/core/k1b/context.h>
#include <arch/core/k1b/cache.h>
#include <arch/core/k1b/mmu.h>
#include <arch/core/k1b/tlb.h>
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
} exceptions[K1B_NUM_EXCEPTIONS + K1B_NUM_EXCEPTIONS_VIRT] = {
	{ K1B_EXCEPTION_RESET,           "reset exception"                              },
	{ K1B_EXCEPTION_OPCODE,          "bad instruction bundle"                       },
	{ K1B_EXCEPTION_PROTECTION,      "protection fault"                             },
	{ K1B_EXCEPTION_ALIGNMENT,       "alignment check exception"                    },
	{ K1B_EXCEPTION_RANGE_CODE,      "instruction out of range"                     },
	{ K1B_EXCEPTION_RANGE_DATA,      "data out of range"                            },
	{ K1B_EXCEPTION_OPCODE,          "double ecc fault on out of range instruction" },
	{ K1B_EXCEPTION_DOUBLE_ECC_DATA, "double ecc fault on out of range data"        },
	{ K1B_EXCEPTION_PARITY_CODE,     "parity error on out of range instruction"     },
	{ K1B_EXCEPTION_PARITY_DATA,     "parity error on out of range data"            },
	{ K1B_EXCEPTION_SINGLE_ECC_CODE, "single ecc fault on out of range instruction" },
	{ K1B_EXCEPTION_SINGLE_ECC_DATA, "single ecc fault on out of range data"        },
	{ K1B_EXCEPTION_TLB_FAULT,       "tlb fault"                                    },
	{ K1B_EXCEPTION_PAGE_PROTECTION, "page protection"                              },
	{ K1B_EXCEPTION_WRITE_CLEAN ,    "write to clean exception"                     },
	{ K1B_EXCEPTION_ATOMIC_CLEAN,    "atomic to clean exception"                    },
	{ K1B_EXCEPTION_VIRT_PAGE_FAULT, "page fault"                                   }
};

/**
 * @brief Exception handlers.
 *
 * Lookup table with registered exception handlers.
 */
PRIVATE k1b_exception_handler_fn k1b_excp_handlers[K1B_NUM_EXCEPTIONS + K1B_NUM_EXCEPTIONS_VIRT] = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL
};

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
	k1b_context_dump(ctx);
	kpanic("unhandled %s exception at %x\n", exceptions[excp->num].errmsg, excp->ea);
}

/**
 * The do_excp() function dispatches an exception to the registered
 * exception handler.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void do_excp(const struct exception *excp, const struct context *ctx)
{
	/* Unknown exception. */
	if (excp->num >= (K1B_NUM_EXCEPTIONS + K1B_NUM_EXCEPTIONS_VIRT))
		kpanic("unknown exception %x\n", excp->num);

	/* Unhandled exception. */
	if (k1b_excp_handlers[excp->num] == NULL)
		do_generic_excp(excp, ctx);

	k1b_excp_handlers[excp->num](excp, ctx);
}

/**
 * @todo Document this function.
 */
PUBLIC void forward_excp(int num, const struct exception *excp, const struct context *ctx)
{
	struct exception *_excp = (struct exception *)excp;

	_excp->num = num;

	do_excp(_excp, ctx);
}

/**
 * The k1b_excp_set_handler() function sets a handler function for
 * the exception @p num.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_excp_set_handler(int num, k1b_exception_handler_fn handler)
{
	/* Invalid exception number. */
	if ((num < 0) || (num >= (K1B_NUM_EXCEPTIONS + K1B_NUM_EXCEPTIONS_VIRT)))
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
	if (k1b_excp_handlers[num] != NULL)
		return (-EBUSY);

	k1b_excp_handlers[num] = handler;
	k1b_dcache_inval();

	return (0);
}

/**
 * The k1b_excp_unset_handler() function unsets a handler function for
 * the exception @p num.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int k1b_excp_unset_handler(int num)
{
	/* Invalid exception number. */
	if ((num < 0) || (num >= (K1B_NUM_EXCEPTIONS + K1B_NUM_EXCEPTIONS_VIRT)))
	{
		kprintf("[hal] invalid exception number");
		return (-EINVAL);
	}

	/* Bad exception number. */
	if (k1b_excp_handlers[num] == NULL)
		return (-EINVAL);

	k1b_excp_handlers[num] = NULL;
	k1b_dcache_inval();

	return (0);
}
