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

#ifndef _HAL_TEST_H_
#define _HAL_TEST_H_

	/**
	* @name Node numbers for stress tests
	*/
	/**@{*/
	#define NODES_AMOUNT   2
	#define NODENUM_MASTER 0
	#ifdef __mppa256__
		#define NODENUM_SLAVE  (PROCESSOR_NODENUM_MASTER + PROCESSOR_NOC_IONODES_NUM)
	#else
		#define NODENUM_SLAVE  (PROCESSOR_NODENUM_MASTER + 1)
	#endif
	/**@}*/

	/**
	 * @brief Unit test.
	 */
	struct test
	{
		void (*test_fn)(void); /**< Test function. */
		const char *name;      /**< Test name.     */
	};

	/**
	 * @brief Horizontal line for tests.
	 */
	EXTERN const char *HLINE;

	/**
	 * @brief Test driver for the Timer Interface.
	 */
	EXTERN void test_timer(void);

	/**
	 * @brief Test driver for the Core Interface.
	 */
	EXTERN void test_core(void);

	/**
	 * @brief Test driver for the Exception Interface.
	 */
	EXTERN void test_exception(void);

	/**
	 * @brief Test driver for Trap Interface.
	 */
	EXTERN void test_trap(void);

	/**
	 * @brief Test driver for the Upcall Interface.
	 */
	EXTERN void test_upcall(void);

	/**
	 * @brief Test driver for Hardware Interrupt Interface.
	 */
	EXTERN void test_interrupt(void);

	/**
	 * @brief Test driver for TLB Interface
	 */
	EXTERN void test_tlb(void);

	/**
	 * @brief Test driver for MMU Interface
	 */
	EXTERN void test_mmu(void);

	/**
	 * @brief Test driver for the Cores Interface of the Cluster AL.
	 */
	EXTERN void test_cluster_cores(void);

	/**
	 * @brief Test driver for Performance Monitor Interface.
	 */
	EXTERN void test_perf(void);

	/**
	 * @brief Test driver for Spinlock Interface.
	 */
	EXTERN void test_spinlock(void);

	/**
	 * @brief Test driver for Timer Interface.
	 */
	EXTERN void test_timer(void);

	/**
	 * @brief Test driver for the Control NoC Interface
	 */
	EXTERN void test_cnoc(void);

	/**
	 * @brief Test driver for the Data NoC Interface
	 */
	EXTERN void test_dnoc(void);

	/**
	 * @brief Test driver for the Sync Interface
	 */
	EXTERN void test_sync(void);

	/**
	 * @brief Test driver for the Mailbox Interface
	 */
	EXTERN void test_mailbox(void);

	/**
	 * @brief Test driver for the Portal Interface
	 */
	EXTERN void test_portal(void);

	/**
	 * @brief Test driver for the Clusters Interface
	 */
	EXTERN void test_clusters(void);

	/**
	 * @brief Test driver for the NoC Interface
	 */
	EXTERN void test_noc(void);

#endif /* _HAL_TEST_H_ */
