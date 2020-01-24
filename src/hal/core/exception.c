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
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>

/*===========================================================================*
 * default_handler()                                                         *
 *===========================================================================*/

/**
 * @brief Generic exception handler.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted context.
 */
PRIVATE NORETURN void default_handler(
	const struct exception *excp,
	const struct context *ctx)
{
	context_dump(ctx);
	exception_dump(excp);

	kpanic("unhandled exception");

	UNREACHABLE();
}

/*===========================================================================*
 * do_exception()                                                            *
 *===========================================================================*/

/**
 * @brief High-level exception dispatcher.
 *
 * @param excp Exception information.
 * @param ctx  Interrupted context.
 *
 * @note This function is called from assembly code.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void do_exception(const struct exception *excp, const struct context *ctx)
{
	int excpnum;
	exception_handler_t handler;

	/* Nothing to do. */
	excpnum = exception_get_num(excp);
	handler = exceptions[excpnum].handler;

	handler(excp, ctx);
}

/*===========================================================================*
 * exception_register()                                                      *
 *===========================================================================*/

/**
 * The exception_register() function registers @p handler as the
 * exception handler for the exception @p excpnum.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int exception_register(int excpnum, exception_handler_t handler)
{
	/* Invalid exception number. */
	if ((excpnum < 0) || (excpnum >= EXCEPTIONS_NUM))
	{
		kprintf("[hal][core] invalid exception number");
		return (-EINVAL);
	}

	/* Invalid handler. */
	if (handler == NULL)
	{
		kprintf("[hal][core] invalid exception handler");
		return (-EINVAL);
	}

	/* Overwriting handler. */
	if (exceptions[excpnum].handler != default_handler)
	{
		if (exceptions[excpnum].handler != NULL)
		{
			kprintf("[hal][core] overwriting handler %x for %s",
				exceptions[excpnum].handler,
				exceptions[excpnum].name
			);
		}
	}

	exceptions[excpnum].handler = handler;
	dcache_invalidate();

	kprintf("[hal][core] exception handler %x registered for %s",
		exceptions[excpnum].handler,
		exceptions[excpnum].name
	);

	return (0);
}

/*===========================================================================*
 * exception_unregister()                                                    *
 *===========================================================================*/

/**
 * The exception_unregister() function unregisters @p handler as the
 * exception handler for the execption @p excpnum.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC int exception_unregister(int excpnum)
{
	/* Invalid exception number. */
	if ((excpnum < 0) || (excpnum >= EXCEPTIONS_NUM))
	{
		kprintf("[hal][core] invalid exception number");
		return (-EINVAL);
	}

	/* Bad exception number. */
	if (exceptions[excpnum].handler == default_handler)
		return (-EINVAL);

	exceptions[excpnum].handler = default_handler;
	dcache_invalidate();

	return (0);
}

/*===========================================================================*
 * exception_forward()                                                       *
 *===========================================================================*/

/**
 * The exception_forward() function forwards an exception to the
 * handler of exception @p excpnum.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void exception_forward(
	int excpnum,
	const struct exception *excp,
	const struct context *ctx
)
{
	struct exception *_excp = (struct exception *) excp;

	_excp->num = excpnum;

	do_exception(_excp, ctx);
}

/*===========================================================================*
 * exception_setup()                                                         *
 *===========================================================================*/

/**
 * The exception_setuo() function initializes the exception interface.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void exception_setup(void)
{
	for (int i = 0; i < EXCEPTIONS_NUM; i++)
	{
		/* Skip early registered handlers. */
		if (exceptions[i].handler != NULL)
			continue;

		exceptions[i].handler = default_handler;
	}

	dcache_invalidate();
}
