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
#include <nanvix/hlib.h>

/**
 * @cond k1b
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[K1B_EXCP_NUM_EXT] = {
	{ NULL, "reset exception"                              },
	{ NULL, "bad instruction bundle"                       },
	{ NULL, "protection fault"                             },
	{ NULL, "alignment check exception"                    },
	{ NULL, "instruction out of range"                     },
	{ NULL, "data out of range"                            },
	{ NULL, "double ecc fault on out of range instruction" },
	{ NULL, "double ecc fault on out of range data"        },
	{ NULL, "parity error on out of range instruction"     },
	{ NULL, "parity error on out of range data"            },
	{ NULL, "single ecc fault on out of range instruction" },
	{ NULL, "single ecc fault on out of range data"        },
	{ NULL, "tlb fault"                                    },
	{ NULL, "page protection"                              },
	{ NULL, "write to clean exception"                     },
	{ NULL, "atomic to clean exception"                    },
	{ NULL, "page fault"                                   }
};


/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void k1b_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[excp->num].name);
}
