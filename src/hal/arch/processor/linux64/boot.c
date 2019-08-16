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
#define __NEED_PROCESSOR_LINUX64

#include <arch/processor/linux64.h>
#include <arch/stdout/tty-virt.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>
#include <unistd.h>
#include <errno.h>

/*============================================================================*
 * Processor                                                                  *
 *============================================================================*/

/**
 * @brief Boots up the underlying processor.
 */
PRIVATE int linux64_processor_boot(void)
{
	kprintf("[processor] creating virtual processor...");

	linux64_processor_clusters_boot();
	linux64_processor_noc_boot();

	/* Power on clusters. */
	for (int i = 1; i < LINUX64_PROCESSOR_CLUSTERS_NUM; i++)
	{
		pid_t pid;

		/* Cannot power on slave cluster. */
		if ((pid = fork()) < 0)
			return (pid);

		/* Slave cluster. */
		if (pid == 0)
			break;
	}

	return (0);
}

/*============================================================================*
 * main()                                                                     *
 *============================================================================*/

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

	/*
	 * Early initialization of Virtual
	 * TTY device to help us debugging.
	 */
	tty_virt_init();


	/* Boot processor. */
	if ((ret = linux64_processor_boot()) < 0)
		return (ret);

	/* Boot clusters. */
	if ((ret = linux64_cluster_boot()) < 0)
		return (ret);

	linux64_processor_setup();

	return (ret);
}
