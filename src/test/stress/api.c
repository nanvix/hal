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

#include <nanvix/hal/hal.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/errno.h>
#include "../test.h"
#include "stress.h"

#if (__TARGET_HAS_SYNC && __TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX)

/**
 * @brief Stress core fence.
 */
PRIVATE struct fence stress_fence;

/**
 * @brief Launches unit tests on Target AL.
 */
PRIVATE void do_test_stress_al(void)
{
	test_stress_setup();

		test_stress_mailbox();
		test_stress_portal();
		test_stress_combination();

	test_stress_cleanup();

	fence_join(&stress_fence);

	KASSERT(core_release() == 0);
	core_reset();
}

/**
 * The test_stress_al() function launches stress testing units on the
 * mailbox/portal interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_stress_al(void)
{
	test_stress_interrupt_setup();
	fence_init(&stress_fence, 1);

		/* Start a slave core. */
		for (int i = 0; i < CORES_NUM; ++i)
		{
			if (i != COREID_MASTER)
			{
				KASSERT(core_start(i, do_test_stress_al) == 0);
				break;
			}
		}

    fence_wait(&stress_fence);
	test_stress_interrupt_cleanup();
}

#else

/**
 * The test_stress_al() function launches stress testing units on the
 * mailbox/portal interface of the HAL.
 *
 * @author João Vicente Souto
 */
PUBLIC void test_stress_al(void)
{

}

#endif /* __TARGET_HAS_SYNC && __TARGET_HAS_MAILBOX && __TARGET_HAS_PORTAL && !__NANVIX_IKC_USES_ONLY_MAILBOX */
