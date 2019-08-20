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

/* Must come first. */
#define __NEED_CORE_CONTEXT

#include <nanvix/hal/core/interrupt.h>
#include <arch/core/k1b/ctx.h>
#include <arch/core/k1b/cache.h>
#include <arch/core/k1b/int.h>
#include <arch/core/k1b/ivt.h>
#include <nanvix/const.h>
#include <mOS_vcore_u.h>
#include <vbsp.h>

/**
 * @brief High-level interrupt dispatcher.
 */
PRIVATE k1b_int_handler_fn _k1b_do_int = NULL;

/**
 * @brief Interrupt vectors.
 */
PRIVATE struct {
	int intnum; /**< Interrupt Number          */
	int hwint;  /**< Hardware Interrupt Number */
} hwints[K1B_IVT_LENGTH] = {
	{ BSP_IT_TIMER_0, K1B_INT_TIMER0    }, /* Timer 0              */
	{ BSP_IT_TIMER_1, K1B_INT_TIMER1    }, /* Timer 1              */
	{ BSP_IT_WDOG,    K1B_INT_TIMER     }, /* Watchdog Timer       */
	{ BSP_IT_CN,      K1B_INT_CNOC      }, /* Control NoC          */
	{ BSP_IT_RX,      K1B_INT_DNOC      }, /* Data NoC             */
	{ BSP_IT_UC,      K1B_INT_DMA       }, /* DMA                  */
	{ BSP_IT_NE,      K1B_INT_NOC_ERR   }, /* NoC Error            */
	{ BSP_IT_WDOG_U,  K1B_INT_TIMER_ERR }, /* Watchdog Timer Error */
	{ BSP_IT_PE_0,    K1B_INT_VIRT      }, /* Remote Core 0        */
	{ BSP_IT_PE_1,    K1B_INT_VIRT      }, /* Remote Core 1        */
	{ BSP_IT_PE_2,    K1B_INT_VIRT      }, /* Remote Core 2        */
	{ BSP_IT_PE_3,    K1B_INT_VIRT      }, /* Remote Core 3        */
	{ BSP_IT_PE_4,    K1B_INT_VIRT      }, /* Remote Core 4        */
	{ BSP_IT_PE_5,    K1B_INT_VIRT      }, /* Remote Core 5        */
	{ BSP_IT_PE_6,    K1B_INT_VIRT      }, /* Remote Core 6        */
	{ BSP_IT_PE_7,    K1B_INT_VIRT      }, /* Remote Core 7        */
	{ BSP_IT_PE_8,    K1B_INT_VIRT      }, /* Remote Core 8        */
	{ BSP_IT_PE_9,    K1B_INT_VIRT      }, /* Remote Core 9        */
	{ BSP_IT_PE_10,   K1B_INT_VIRT      }, /* Remote Core 10       */
	{ BSP_IT_PE_11,   K1B_INT_VIRT      }, /* Remote Core 11       */
	{ BSP_IT_PE_12,   K1B_INT_VIRT      }, /* Remote Core 12       */
	{ BSP_IT_PE_13,   K1B_INT_VIRT      }, /* Remote Core 14       */
	{ BSP_IT_PE_14,   K1B_INT_VIRT      }, /* Remote Core 14       */
	{ BSP_IT_PE_15,   K1B_INT_VIRT      }, /* Remote Core 15       */
};

/**
 * @brief Low-level interrupt dispatcher.
 *
 * @param intnum Interrupt number.
 * @param ctx    Saved interrupted context.
 *
 * @author Pedro Henrique Penna
 */
PRIVATE void __k1b_do_int(int intnum, __k1_vcontext_t *ctx)
{
	UNUSED(ctx);

	for (int i = 0; i < K1B_IVT_LENGTH; i++)
	{
		if (hwints[i].intnum == intnum)
		{
			if (LIKELY(_k1b_do_int != NULL))
				_k1b_do_int(hwints[i].hwint);
			return;
		}
	}
}

/**
 * @brief Initializes the interrupt vector table.
 *
 * @param hwint_handler Default hardware interrupt handler.
 * @param swint_handler Default software interrupt handler.
 * @param excp_handler  Default exception handler.
 * @param stack         Stack for interrupts, exceptions and traps.
 *
 * The k1b_ivt_setup() function initializes the interrupt vector table
 * in the k1b architecture. It traverses all entries of this table and
 * properly registers @p do_hwintm @p do_swint and do_excp as default
 * handlers for hardware interrupts, software interrupts and
 * exceptions, respectively.
 */
PRIVATE void k1b_ivt_setup(
	k1b_int_handler_fn hwint_handler,
	k1b_swint_handler_fn swint_handler,
	void (*excp_handler)(void),
	void *stack)
{
	kprintf("[hal] exception stack at %x", stack);

	/* Register dispatchers. */
	_k1b_do_int = hwint_handler;
	for (int i = 0; i < K1B_IVT_LENGTH; i++)
		bsp_register_it(__k1b_do_int, hwints[i].intnum);
	mOS_register_scall_handler(swint_handler);
	mOS_register_trap_handler(excp_handler);
	k1b_dcache_inval();

	/* Setup shadow stack for exceptions. */
	mOS_register_stack_handler(stack);
	mOS_trap_enable_shadow_stack();

	k1b_pic_setup();
}

/**
 * @todo TODO provide a detailed description for this function.
 *
 * @author Pedro Henrique Penna
 */
PUBLIC void ivt_setup(void *stack)
{
	k1b_ivt_setup(
		do_interrupt,
		_k1b_do_kcall,
		_k1b_do_excp,
		stack
	);
}
