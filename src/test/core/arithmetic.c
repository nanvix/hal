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
#include "../test.h"

/*============================================================================*
 * API Tests                                                                  *
 *============================================================================*/

/*----------------------------------------------------------------------------*
 * Lock and Unlock a Spinlock                                                 *
 *----------------------------------------------------------------------------*/

#define COMMENT(c)
#define VALUE(type, a) ((type) a)
#define OP(type, operator, a, b) (VALUE(type, a) operator VALUE(type, b))

/**
 * @brief API Test: Genericc arithmetic function.
 *
 * @author João Vicente Souto
 */
#define TEST_ARITHMETIC_FUNC(funcname, type, value_a, value_b) \
PRIVATE void test_api_ ## funcname (void)                      \
{                                                              \
	type a = value_a;                                          \
	type b = value_b;                                          \
                                                               \
	COMMENT(Arithmetic)                                        \
	KASSERT((a + b) == OP(type, +, value_a, value_b));         \
	KASSERT((a - b) == OP(type, -, value_a, value_b));         \
	KASSERT((a * b) == OP(type, *, value_a, value_b));         \
	KASSERT((a / b) == OP(type, /, value_a, value_b));         \
	KASSERT((a % b) == OP(type, %, value_a, value_b));         \
                                                               \
	COMMENT(Bit-wise)                                          \
	KASSERT((a & b)  == OP(type,  &, value_a, value_b));       \
	KASSERT((a | b)  == OP(type,  |, value_a, value_b));       \
	KASSERT((a ^ b)  == OP(type,  ^, value_a, value_b));       \
	KASSERT((a << b) == OP(type, <<, value_a, value_b));       \
	KASSERT((a >> b) == OP(type, >>, value_a, value_b));       \
	KASSERT((~a) == ~VALUE(type, value_a));                    \
                                                               \
	COMMENT(Logic)                                             \
	KASSERT((a == b) == OP(type, ==, value_a, value_b));       \
	KASSERT((a != b) == OP(type, !=, value_a, value_b));       \
	KASSERT((a < b)  == OP(type,  <, value_a, value_b));       \
	KASSERT((a <= b) == OP(type, <=, value_a, value_b));       \
	KASSERT((a > b)  == OP(type,  >, value_a, value_b));       \
	KASSERT((a >= b) == OP(type, >=, value_a, value_b));       \
	KASSERT((!a) == !VALUE(type, value_a));                    \
}

/**
 * @name Group of arithmetic test functions.
 */
/**{**/
TEST_ARITHMETIC_FUNC(byte, byte_t, 8, 3)
TEST_ARITHMETIC_FUNC(hword, hword_t, 8, 3)
TEST_ARITHMETIC_FUNC(word, word_t, 27, 5)
TEST_ARITHMETIC_FUNC(char, char, 8, 3)
TEST_ARITHMETIC_FUNC(short, short, 8, 3)
TEST_ARITHMETIC_FUNC(int, int, 27, 5)
TEST_ARITHMETIC_FUNC(unsigned_int, unsigned int, 27, 5)
TEST_ARITHMETIC_FUNC(long, long, 27, 5)
TEST_ARITHMETIC_FUNC(unsigned_long, unsigned long, 27, 5)
TEST_ARITHMETIC_FUNC(int32_t, int32_t, 49153, 5)
TEST_ARITHMETIC_FUNC(uint32_t, uint32_t, 49153, 5)
/**}**/

/*============================================================================*
 * Test Driver                                                                *
 *============================================================================*/

/**
 * @brief Unit tests.
 */
PRIVATE struct test test_api_arithmetic[] = {
	{ test_api_byte,               "byte_t operations            "},
	{ test_api_hword,              "hword_t operations           "},
	{ test_api_word,               "word_t operations            "},
	{ test_api_char,               "char operations              "},
	{ test_api_short,              "short operations             "},
	{ test_api_int,                "int operations               "},
	{ test_api_unsigned_int,       "unsigned int operations      "},
	{ test_api_long,               "long operations              "},
	{ test_api_unsigned_long,      "unsigned long operations     "},
	{ test_api_int32_t,            "int32_t operations           "},
	{ test_api_uint32_t,           "uint32_t operations          "},
	{ NULL,                         NULL                          },
};

/**
 * The test_arithmetic() function perform arithmetic operations.
 *
 * @author João Vicente Souto
" */
PUBLIC void test_arithmetic(void)
{
	/* API Tests */
	kprintf(HLINE);
	for (int i = 0; test_api_arithmetic[i].test_fn != NULL; i++)
	{
		test_api_arithmetic[i].test_fn();
		kprintf("[test][api][arithmetic] %s [passed]", test_api_arithmetic[i].name);
	}
}

