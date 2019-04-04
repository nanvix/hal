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
#define __NEED_CORE_PIC

#include <arch/core/rv32i/pic.h>
#include <nanvix/const.h>

/**
 * @brief Table of Interrupt Requests (IRQs).
 */
PUBLIC const int irqs[RV32I_IRQ_NUM] = {
	RV32I_MIE_USIE, /* User Software Interrupt Enable       */
	RV32I_MIE_SSIE, /* Supervisor Interrupt Enable          */
	0,
	RV32I_MIE_MSIE, /* Machine Software Interrupt Enable    */
	RV32I_MIE_UTIE, /* User Timer Interrupt Enable          */
	RV32I_MIE_STIE, /* Supervisor Timer Interrupt Enable    */
	0,
	RV32I_MIE_MTIE, /* Machine Timer Interrupt Enable       */
	RV32I_MIE_UEIE, /* User External Interrupt Enable       */
	RV32I_MIE_SEIE, /* Supervisor External Interrupt Enable */
	0,
	RV32I_MIE_MEIE  /* Machine External Interrupt Enable    */
};
