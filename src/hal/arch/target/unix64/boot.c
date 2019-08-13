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

/* Must come fist. */
#define __NEED_TARGET_UNIX64

#include <arch/target/unix64/unix64.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @brief Powers on the underlying target.
 */
PRIVATE int unix64_boot(void)
{
	/*
	 * Early initialization of Virtual
	 * TTY device to help us debugging.
	 */
	tty_virt_init();

	kprintf("[hal][target] powering on target...");

	return (linux64_processor_boot());
}

/**
 * @brief Entry point.
 *
 * @param argc Number of arguments.
 * @param argv Arguments list.
 */
int main(int argc, char **argv)
{
	int ret;

	UNUSED(argc);
	UNUSED(argv);

	/* Boot processor. */
	if ((ret = unix64_boot()) < 0)
		return (ret);

	unix64_setup();

	return (0);
}
