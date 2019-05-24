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

#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <arch/stdout/tty-virt.h>

/* Import definitions. */
EXTERN NORETURN void kmain(int, const char *[]);

/*============================================================================*
 * linux64_cluster_setup()                                                    *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void linux64_cluster_setup(void)
{
	// TODO: implement
}

/*============================================================================*
 * linux64_cluster_slave_setup()                                              *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void linux64_cluster_slave_setup(void)
{
	UNREACHABLE();
}

/*============================================================================*
 * linux64_cluster_master_setup()                                             *
 *============================================================================*/

/**
 * @todo TODO provide a detailed description of this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC NORETURN void linux64_cluster_master_setup(void)
{
	/*
	 * Early initialization of Virtual
	 * TTY device to help us debugging.
	 */
	tty_virt_init();

	/* cluster setup. */
	linux64_cluster_setup();

	kmain(0, NULL);
}
