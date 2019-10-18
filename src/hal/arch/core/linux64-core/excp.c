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

#include <nanvix/hal/core/exception.h>
#include <arch/core/linux64/ctx.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

EXTERN struct context linux64_create_context(void);

/**
 * @brief Generic handler of an exception
 */
PRIVATE void linux64_do_exception(const struct exception *excp, const struct context *ctx)
{
	kprintf("<%s> detected ! (%d)", exceptions[exception_get_num(excp)].name, exception_get_num(excp));
	context_dump(ctx);
	exit(0);
}

/**
 * @brief List of the signal considered as exceptions
 */
PRIVATE int linux64_excp_signals[] = {
	SIGFPE,
	SIGTRAP,
	SIGSEGV,
	SIGILL,
	SIGBUS,
	-1
};

/**
 * @cond linux64
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[LINUX64_EXCP_NUM] = {
	{ NULL,                  NULL                                               },
	{ NULL,                  NULL                                               },
	{ NULL,                  NULL                                               },
	{ NULL,                  NULL                                               },
	{ linux64_do_exception, "Illegal instruction"                               },
	{ linux64_do_exception, "Trace/breakpoint trap"                             },
	{ NULL,                  NULL                                               },
	{ NULL,                  NULL                                               },
	{ linux64_do_exception, "Erroneous arithmetic operation"                    },
	{ NULL,                  NULL                                               },
	{ linux64_do_exception, "Access to an undefined portion of a memory object" },
	{ linux64_do_exception, "Invalid memory reference"                          }
};

/**
 * @brief Exception Handler called by the signal() function
 */
void linux64_excp_handler(int excpnum)
{
	struct exception excp;
	kmemset(&excp, 0, sizeof(struct exception));
	excp.num = excpnum;
	struct context ctx = linux64_create_context();
	do_exception(&excp, &ctx);
}

/**
 * @brief Setup the signals
 */
PUBLIC void linux64_excp_setup(void)
{
	for(int i = 0; linux64_excp_signals[i] != -1; i++)
		signal(linux64_excp_signals[i], linux64_excp_handler);
}

/**
 * @brief Dumps information about an exception.
 *
 * @param excp Exception information.
 */
PUBLIC void linux64_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[exception_get_num(excp)].name);
}
