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

#ifndef _STRESS_H_
#define _STRESS_H_

	/**
	 * @name Auxiliar functions of Stress Tests
	 */
	/**@{*/
	EXTERN void test_stress_interrupt_setup(void);
	EXTERN void test_stress_interrupt_cleanup(void);
	EXTERN void test_stress_setup(void);
	EXTERN void test_stress_cleanup(void);
	EXTERN void test_stress_barrier(void);
	/**@}*/

	/**
	 * @brief Stress test driver for the Mailbox Interface
	 */
	EXTERN void test_stress_mailbox(void);

	/**
	 * @brief Stress test driver for the Portal Interface
	 */
	EXTERN void test_stress_portal(void);

	/**
	 * @brief Stress test driver for the combination of Mailbox and Portal Interfaces
	 */
	EXTERN void test_stress_combination(void);

#endif /* _STRESS_H_ */
