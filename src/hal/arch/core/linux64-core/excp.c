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

#include <nanvix/hal/core/exception.h>
#include <arch/core/linux64/ctx.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/*
 * XXX This belongs to the Cluster AL. We should find a nicer
 * way of doing this ot avoid any possible circular dependency.
 */
EXTERN int linux64_core_get_id(void);

/**
 * @cond linux64
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[LINUX64_EXCP_NUM] = {
	{ NULL, ""                               },
	{ NULL, ""                               },
	{ NULL, ""                               },
	{ NULL, ""                               },
	{ NULL, "illegal instruction"            },
	{ NULL, "trace/breakpoint trap"          },
	{ NULL, ""                               },
	{ NULL, ""                               },
	{ NULL, "erroneous arithmetic operation" },
	{ NULL, ""                               },
	{ NULL, "segmentation fault"             },
	{ NULL, "invalid memory reference"       }
};

/**
 * @brief Exception Handler called by the signal() function
 */
PRIVATE void linux64_excp_handler(int excpnum)
{
	struct context ctx;
	struct exception excp;

	excp.num = excpnum;
	ctx.id = linux64_core_get_id();

	do_exception(&excp, &ctx);

	context_dump(&ctx);
	exception_dump(&excp);
	kpanic("unhandled exception");
	UNREACHABLE();
}

/**
 * @brief List of the signal considered as exceptions
 */
PRIVATE struct {
	int num;
	void (*handler)(int);

} linux64_signals[] = {
	{ SIGFPE,  linux64_excp_handler },
	{ SIGTRAP, linux64_excp_handler },
	{ SIGSEGV, linux64_excp_handler },
	{ SIGILL,  linux64_excp_handler },
	{ SIGBUS,  linux64_excp_handler },
	{ -1,      NULL                 }
};

/**
 * @brief Setup the signals
 */
PUBLIC void linux64_excp_setup(void)
{
	for (int i = 0; linux64_signals[i].num != -1; i++)
		KASSERT(signal(linux64_signals[i].num, linux64_signals[i].handler) != SIG_ERR);
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
