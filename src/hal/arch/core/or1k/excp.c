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

#include <nanvix/hal/core/exception.h>
#include <nanvix/const.h>
#include <nanvix/klib.h>

/**
 * @cond or1k
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[OR1K_EXCP_NUM] = {
	{ NULL, "reset exception"           },
	{ NULL, "bus error"                 },
	{ NULL, "page fault"                },
	{ NULL, "alignment check exception" },
	{ NULL, "illegal instruction"       },
	{ NULL, "data tlb fault"            },
	{ NULL, "instruction tlb fault"     },
	{ NULL, "data out of range"         },
	{ NULL, "float point exception"     },
	{ NULL, "trap exception"            }
};

/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void or1k_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[excp->num].name);
}
