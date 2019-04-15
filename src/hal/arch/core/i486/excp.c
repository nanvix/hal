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
 * @cond i486
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[I486_EXCP_NUM] = {
	{ NULL, "division-by-zero error"        },
	{ NULL, "debug exception"               },
	{ NULL, "non-maskable interrupt"        },
	{ NULL, "breakpoint exception"          },
	{ NULL, "overflow exception"            },
	{ NULL, "bounds check exception"        },
	{ NULL, "invalid opcode exception"      },
	{ NULL, "coprocessor not available"     },
	{ NULL, "double fault"                  },
	{ NULL, "coprocessor segment overrun"   },
	{ NULL, "invalid task state segment"    },
	{ NULL, "segment not present"           },
	{ NULL, "static segment fault"          },
	{ NULL, "general protection fault"      },
	{ NULL, "page fault"                    },
	{ NULL, "floating point unit exception" },
	{ NULL, "alignment check"               },
	{ NULL, "machine exception"             },
	{ NULL, "smid unit exception"           },
	{ NULL, "virtual exception"             },
	{ NULL, "security exception"            },
};

/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void i486_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[excp->num].name);
}
