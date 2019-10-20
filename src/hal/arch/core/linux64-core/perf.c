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

/* Must come first. */
#define __NEED_CORE_TYPES

#include <arch/core/linux64/types.h>
#include <arch/core/linux64/perf.h>
#include <nanvix/hlib.h>
#include <sys/ioctl.h>
#include <syscall.h>
#include <posix/stdint.h>
#include <unistd.h>
#include <posix/errno.h>

/**
 * @Brief Function to call to configure a perf
 */
PRIVATE int perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
	return (syscall(__NR_perf_event_open, (linux64_dword_t) hw_event, pid, cpu, group_fd, flags));
}

/**
 * @Brief Array of File Descriptor containing the values of the counters
 */
int linux64_perf_monitors[LINUX64_PERF_MONITORS_NUM];


/**
 * @Brief Check if the perf and event values are valide
 */
PUBLIC int perf_isvalid(int perf)
{
	return (perf >= 0 && perf < LINUX64_PERF_MONITORS_NUM);
}

PUBLIC int event_isvalid(int event)
{
	return (event >= 0 && event < LINUX64_PERF_EVENTS_NUM);
}

/**
 * @brief Initializes performance monitors.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC void linux64_perf_setup(void)
{
	for(int i = 0; i < LINUX64_PERF_MONITORS_NUM; i++)
		linux64_perf_monitors[i] = -1;
}

/**
 * @brief Starts a performance monitor.
 *
 * @param perf  Target performance monitor.
 * @param event Target event to watch.
 *
 * @returns Upon successful completion, zero is returned.
 * Upon failure, a negative error code is returned instead.
 */
PUBLIC int linux64_perf_start(int perf, int event)
{
	if (!perf_isvalid(perf) || !event_isvalid(event))
		return (-EINVAL);

	struct perf_event_attr attr;
	kmemset(&attr, 0, sizeof(struct perf_event_attr));
	attr.type = PERF_TYPE_HARDWARE;
	attr.size = sizeof(struct perf_event_attr);
	attr.config = event;
	attr.disabled = 1;
	attr.exclude_kernel = 1;
	attr.exclude_hv = 1;
	linux64_perf_monitors[perf] = perf_event_open(&attr,
												LINUX64_PERF_ARG1,
												LINUX64_PERF_ARG2,
												LINUX64_PERF_ARG3,
												LINUX64_PERF_ARG4);

	if (linux64_perf_monitors[perf] == -1)
		return (-EINVAL);

	ioctl(linux64_perf_monitors[perf], PERF_EVENT_IOC_RESET, 0);
	ioctl(linux64_perf_monitors[perf], PERF_EVENT_IOC_ENABLE, 0);
	return (0);
}

/**
 * @brief Stops a performance monitor.
 *
 * @param perf Target performance monitor.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int linux64_perf_stop(int perf)
{
	if (!perf_isvalid(perf))
		return (-EINVAL);

	ioctl(linux64_perf_monitors[perf], PERF_EVENT_IOC_DISABLE, 0);
	return (0);
}

/**
 * @brief Restarts a performance monitor.
 *
 * @param perf Target performance monitor.
 *
 * @returns Upon successful completion, zero is returned. Upon
 * failure, a negative error code is returned instead.
 */
PUBLIC int linux64_perf_restart(int perf)
{
	if (!perf_isvalid(perf))
		return (-1);

	ioctl(linux64_perf_monitors[perf], PERF_EVENT_IOC_RESET, 0);
	return (0);
}

/**
 * @brief Reads a PM register.
 *
 * @param perf Target performance monitor.
 *
 * @returns Upon successful completion, the value of the target
 * performance monitor.
 */
PUBLIC uint64_t linux64_perf_read(int perf)
{
	if (!perf_isvalid(perf))
		return (-1);

	uint64_t res = 0;
	if (read(linux64_perf_monitors[perf], &res, sizeof(unsigned long)) < 1)
		return (0);
	else
		return (res);
}
