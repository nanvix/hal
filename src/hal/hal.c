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

#include <nanvix/hal/hal.h>
#include <nanvix/klib.h>

/* Size checking. */
#if (CONTEXT_SIZE % WORD_SIZE)
#error "size of context structure should be multiple of a word"
#endif

/**
 * @brief The hal_init() function performs the initialization of the
 * HAL, by starting up the following essential modules:
 *
 * - Log System
 * - Interrupt System
 *
 * The overlying kernel should call hal_init() before using the HAL.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void hal_init(void)
{
	hal_log_setup();

	/* Sanity check size of structures. */
	KASSERT_SIZE(sizeof(struct context), CONTEXT_SIZE);
	KASSERT(ALIGNED(sizeof(struct context), DWORD_SIZE));
	KASSERT_SIZE(sizeof(struct exception), EXCEPTION_SIZE);
	KASSERT(ALIGNED(sizeof(struct exception), DWORD_SIZE));

	exception_setup();
	interrupt_setup();
	processor_noc_setup();
}
