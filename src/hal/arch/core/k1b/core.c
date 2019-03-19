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

#include <nanvix/hal/cluster/cpu.h>
#include <nanvix/const.h>

/**
 * @brief Cores table.
 */
PUBLIC struct coreinfo ALIGN(K1B_CACHE_LINE_SIZE) cores[K1B_NUM_CORES] = {
	{ TRUE,  CORE_RUNNING,   0, NULL, K1B_SPINLOCK_UNLOCKED }, /* Master Core   */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 1  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 2  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 3  */
#if defined(__node__)
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 4  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 5  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 6  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 7  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 8  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 9  */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 10 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 11 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 12 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 13 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 14 */
	{ FALSE, CORE_RESETTING, 0, NULL, K1B_SPINLOCK_LOCKED   }, /* Slave Core 15 */
#endif
};
