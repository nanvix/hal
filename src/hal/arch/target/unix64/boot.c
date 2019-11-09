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
#define __NEED_HAL_TARGET

#include <nanvix/hal/target.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Boot arguments.
 */
PRIVATE struct
{
	int nclusters; /**< Number of Clusters */
} boot_args = {
	1
};

/**
 * @brief Parses boot arguments.
 *
 * @param argc Argument count.
 * @param argv Argument list.
 */
PRIVATE void unix64_parse_boot_args(int argc, const char **argv)
{
	for (int i = 1; i < argc; /* noop*/)
	{
		/* Unkonwn argument. */
		if (strcmp(argv[i], "--nclusters"))
			exit(-EINVAL);

		/* Missing argument. */
		if ((i + 1) > argc)
			exit(-EINVAL);

		sscanf(argv[i + 1], "%d", &boot_args.nclusters);

		fprintf(stderr, "[unix64] argv[%d]: %s %s\n", i, argv[i], argv[i + 1]);

		i += 2;
	}

	/* Bad argument. */
	if ((boot_args.nclusters < 1) || (boot_args.nclusters > PROCESSOR_CLUSTERS_NUM))
		exit(-EINVAL);
}

/**
 * @brief Powers on the underlying target.
 *
 * @param nclusters Number of clusters to power on.
 */
PRIVATE int unix64_boot(int nclusters)
{
	/*
	 * Early initialization of Virtual
	 * TTY device to help us debugging.
	 */
	tty_virt_init();

	kprintf("[hal][target] powering on...");

	return (linux64_processor_boot(nclusters));
}

/**
 * @todo TODO: provide a detailed description for this function.
 */
PUBLIC NORETURN void unix64_poweroff(void)
{
	kprintf("[hal][target] powering off...");

	unix64_sync_shutdown();
	unix64_mailbox_shutdown();
	unix64_portal_shutdown();

	processor_poweroff();
}

/**
 * @brief Entry point.
 *
 * @param argc Number of arguments.
 * @param argv Arguments list.
 */
int main(int argc, const char **argv)
{
	int error;

	unix64_parse_boot_args(argc, argv);

	/* Boot processor. */
	if ((error = unix64_boot(boot_args.nclusters)) < 0)
		return (error);

	unix64_setup();

	UNREACHABLE();

	return (0);
}
