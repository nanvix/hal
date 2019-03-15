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

#ifndef _HAL_TEST_H_
#define _HAL_TEST_H_

	/**
	 * @brief Unit test.
	 */
	struct test
	{
		void (*test_fn)(void); /**< Test function. */
		const char *name;      /**< Test name.     */
	};

	/**
	 * @brief Test driver for the Clock Interface.
	 */
	EXTERN void test_clock(void);

	/**
	 * @brief Test driver for the Core Interface.
	 */
	EXTERN void test_core(void);

	/**
	 * @brief Test driver for the sync interface
	 */
	EXTERN void test_sync(void);

	/**
	 * @brief Test driver for the Exception Interface.
	 */
	EXTERN void test_exception(void);

	/**
	 * @brief Test driver for trap interface.
	 */
	EXTERN void test_trap(void);

#endif /* _HAL_TEST_H_ */
