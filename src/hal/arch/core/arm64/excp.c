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
#include <arch/core/arm64/excp.h>
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
	{ NULL, "Unknown reason." 													}, //< 0
	{ NULL, "Trapped WFI."       												}, //< 1
	{ NULL, ""                               									}, //< 2
	{ NULL, ""                               									}, //< 3
	{ NULL, ""                               									}, //< 4
	{ NULL, ""                               									}, //< 5
	{ NULL, "Trapped LDC or STC access."            							}, //< 6
	{ NULL, "Access to SVE, Advanced SIMD, or floating-point functionality."    }, //< 7
	{ NULL, ""                               									}, //< 8
	{ NULL, ""                               									}, //< 9
	{ NULL, ""                               									}, //< 10
	{ NULL, ""                               									}, //< 11
	{ NULL, ""                               									}, //< 12
	{ NULL, "Branch Target Exception."        									}, //< 13
	{ NULL, "Illegal Execution state."              							}, //< 14
	{ NULL, ""                               									}, //< 15
	{ NULL, ""                               									}, //< 16
	{ NULL, ""                               									}, //< 17
	{ NULL, ""                               									}, //< 18
	{ NULL, ""                               									}, //< 19
	{ NULL, ""                               									}, //< 20
	{ NULL, "SVC instruction execution in AArch64 state."   					}, //< 21
	{ NULL, "HVC instruction execution in AArch64 state."         				}, //< 22
	{ NULL, "SMC instruction execution in AArch64 state."   					}, //< 23
	{ NULL, "Trapped MSR, MRS or System instruction execution in AArch64."   	}, //< 24
	{ NULL, "Access to SVE functionality."                       				}, //< 25
	{ NULL, ""                               									}, //< 26
	{ NULL, ""                               									}, //< 27
	{ NULL, ""                               									}, //< 28
	{ NULL, ""                               									}, //< 29
	{ NULL, ""                               									}, //< 30
	{ NULL, ""                               									}, //< 31
	{ NULL, "Instruction Abort from a lower Exception level."   				}, //< 32
	{ NULL, "Instruction Abort taken without a change in Exception level."      }, //< 33
	{ NULL, "PC alignment fault exception."                						}, //< 34
	{ NULL, ""                               									}, //< 35
	{ NULL, "Data Abort from a lower Exception level."                       	}, //< 36
	{ NULL, "Data Abort taken without a change in Exception level."             }, //< 37
	{ NULL, "SP alignment fault exception."                						}, //< 38
	{ NULL, ""                               									}, //< 39
	{ NULL, ""                               									}, //< 40
	{ NULL, ""                               									}, //< 41
	{ NULL, ""                               									}, //< 42
	{ NULL, ""                               									}, //< 43
	{ NULL, "Trapped floating-point exception taken from AArch64."             	}, //< 44
	{ NULL, ""                               									}, //< 45
	{ NULL, ""                               									}, //< 46
	{ NULL, "SError interrupt."                									}, //< 47
	{ NULL, "Breakpoint exception from a lower Exception level."             	}, //< 48
	{ NULL, "Breakpoint exception taken without a change in Exception level."   }, //< 49
	{ NULL, "Software Step exception from a lower Exception level."             }, //< 50
	{ NULL, "Software Step exception taken without a change in Exception level."}, //< 51
	{ NULL, "Watchpoint exception from a lower Exception level."             	}, //< 52
	{ NULL, "Watchpoint exception taken without a change in Exception level."   }, //< 53
	{ NULL, ""                               									}, //< 54
	{ NULL, ""                               									}, //< 55
	{ NULL, ""                               									}, //< 56
	{ NULL, ""                               									}, //< 57
	{ NULL, ""                               									}, //< 58
	{ NULL, ""                               									}, //< 59
	{ NULL, "BRK instruction execution in AArch64 state."             			}, //< 60
	{ NULL, "Page Fault"             											}, //< 61
	{ NULL, "Page Protection"             										}, //< 62
	{ NULL, "General Protection"             									}  //< 63
};

/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void arm64_handle_exception(const struct context *ctx, arm64_word_t excp_cause, arm64_word_t fault_addr){
	struct exception excp;


	excp.num = excp_cause >> ARM64_EXCP_CAUSE_SHIFT;
	excp.code = excp_cause;
	excp.addr = fault_addr;
	arm64_context_dump(ctx);
	kprintf("[arm64][excp] exception cause %x", excp.num);
	kprintf("[arm64][excp] exception code %x", excp.code);
	kprintf("[arm64][excp] fault adrr %x", excp.addr);
	kprintf("[arm64] unhandled exception");
	//do_exception(&excp, ctx);
	UNREACHABLE();
}

/**
 * @todo TODO provide a detailed description for this function.
 */
PUBLIC void arm64_excp_dump(const struct exception *excp)
{
	kprintf("%s", exceptions[excp->num].name);
}
