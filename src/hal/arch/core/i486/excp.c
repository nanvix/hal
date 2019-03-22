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

#include <arch/core/i486/context.h>
#include <arch/core/i486/excp.h>
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
	int num;            /**< Number.        */
	const char *errmsg; /**< Error message. */
} exceptions[I486_NUM_EXCEPTIONS] = {
	{ I486_EXCEPTION_DIVIDE,                      "division-by-zero error"        },
	{ I486_EXCEPTION_DEBUG,                       "debug exception"               },
	{ I486_EXCEPTION_NMI,                         "non-maskable interrupt"        },
	{ I486_EXCEPTION_BREAKPOINT,                  "breakpoint exception"          },
	{ I486_EXCEPTION_OVERFLOW,                    "overflow exception"            },
	{ I486_EXCEPTION_BOUNDS,                      "bounds check exception"        },
	{ I486_EXCEPTION_INVALID_OPCODE,              "invalid opcode exception"      },
	{ I486_EXCEPTION_COPROCESSOR_NOT_AVAILABLE,   "coprocessor not available"     },
	{ I486_EXCEPTION_DOUBLE_FAULT,                "double fault"                  },
	{ I486_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN, "coprocessor segment overrun"   },
	{ I486_EXCEPTION_INVALID_TSS,                 "invalid task state segment"    },
	{ I486_EXCEPTION_SEGMENT_NOT_PRESENT,         "segment not present"           },
	{ I486_EXCEPTION_STACK_SEGMENT_FAULT,         "static segment fault"          },
	{ I486_EXCEPTION_GENERAL_PROTECTION,          "general protection fault"      },
	{ I486_EXCEPTION_PAGE_FAULT,                  "page fault"                    },
	{ I486_EXCEPTION_FPU_ERROR,                   "floating point unit exception" },
	{ I486_EXCEPTION_ALIGNMENT_CHECK,             "alignment check"               },
	{ I486_EXCEPTION_MACHINE_CHECK,               "machine exception"             },
	{ I486_EXCEPTION_SIMD_ERROR,                  "smid unit exception"           },
	{ I486_EXCEPTION_VIRTUAL_EXCEPTION,           "virtual exception"             },
	{ I486_EXCEPTION_SECURITY_EXCEPTION,          "security exception"            }
};

/**
 * @brief Exception handlers.
 *
 * Lookup table with registered exception handlers.
 */
PRIVATE i486_exception_handler_fn i486_excp_handlers[I486_NUM_EXCEPTIONS] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/**
 * @brief Generic exception handler.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted context.
 */
PRIVATE void generic_excp_handler(const struct exception *excp, const struct context *ctx)
{
	/* Dump execution context. */
	kprintf("[i486] eax=%x ebx=%x ecx=%x edx=%x", ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);
	kprintf("[i486] esi=%x edi=%x ebp=%x esp=%x", ctx->esi, ctx->edi, ctx->ebp, ctx->useresp);
	kprintf("[i486]  cs=%x  ds=%x  ss=%x", 0xff & ctx->cs, 0xff & ctx->ds, 0xff & ctx->ss);
	kprintf("[i486]  es=%x  fs=%x  gs=%x", 0xff & ctx->es, 0xff & ctx->fs, 0xff & ctx->gs);
	kprintf("[i486] eip=%x eflags=%x", ctx->eip, ctx->eflags);

	/* Dump exception information. */
	kpanic("%s", exceptions[excp->num].errmsg);
}

/**
 * @brief High-level exception dispatcher.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted context.
 *
 * @note This function is called from assembly code.
 */
PUBLIC void do_excp(const struct exception *excp, const struct context *ctx)
{
	/* Nothing to do. */
	if (i486_excp_handlers[excp->num] == NULL)
		generic_excp_handler(excp, ctx);

	kprintf("forwarding exception");

	i486_excp_handlers[excp->num](excp, ctx);
}

/**
 * The i486_excp_set_handler() function sets a handler function for
 * the exception @p num.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int i486_excp_set_handler(int num, i486_exception_handler_fn handler)
{
	/* Invalid exception number. */
	if ((num < 0) || (num >= I486_NUM_EXCEPTIONS))
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
	if (i486_excp_handlers[num] != NULL)
		return (-EBUSY);

	i486_excp_handlers[num] = handler;

	return (0);
}
