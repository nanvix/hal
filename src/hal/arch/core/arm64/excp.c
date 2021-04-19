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

#include <nanvix/hal/core/exception.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>

/**
 * @cond arm64
 *
 * Lookup table with information about exceptions.
 *
 * @endcond
 */
PUBLIC struct exception_info exceptions[ARM64_EXCP_NUM_EXT] = {
	{ NULL, "Unknown reason." 													},
	{ NULL, "Trapped WFI or WFE instruction execution."       					},
	{ NULL, "Trapped LDC or STC access."            							},
	{ NULL, "Access to SVE, Advanced SIMD, or floating-point functionality."    },
	{ NULL, "Trapped VMRS access."        										},
	{ NULL, "Illegal Execution state."              							},
	{ NULL, "SVC instruction execution in AArch64 state."   					},
	{ NULL, "HVC instruction execution in AArch64 state."         				},
	{ NULL, "SMC instruction execution in AArch64 state."   					},
	{ NULL, "Trapped MSR, MRS or System instruction execution in AArch64."   	},
	{ NULL, "Access to SVE functionality."                       				},
	{ NULL, "Instruction Abort from a lower Exception level."   				},
	{ NULL, "Instruction Abort taken without a change in Exception level."      },
	{ NULL, "PC alignment fault exception."                						},
	{ NULL, "Data Abort from a lower Exception level."                       	},
	{ NULL, "Data Abort taken without a change in Exception level."             },
	{ NULL, "SP alignment fault exception."                						},
	{ NULL, "Trapped floating-point exception taken from AArch64."             	},
	{ NULL, "SError interrupt."                									},
	{ NULL, "Breakpoint exception from a lower Exception level."             	},
	{ NULL, "Breakpoint exception taken without a change in Exception level."   },
	{ NULL, "Software Step exception from a lower Exception level."             },
	{ NULL, "Software Step exception taken without a change in Exception level."},
	{ NULL, "Watchpoint exception from a lower Exception level."             	},
	{ NULL, "Watchpoint exception taken without a change in Exception level."   },
	{ NULL, "BRK instruction execution in AArch64 state."             			},
	{ NULL, "Page Fault"             											},
	{ NULL, "Page Protection"             										},
	{ NULL, "General Protection"             									}
};

/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void arm64_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[excp->num].name);
}
