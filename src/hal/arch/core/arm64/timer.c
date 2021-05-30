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

#include <arch/core/arm64/timer.h>
#include <arch/core/arm64/gic.h>
#include <nanvix/const.h>
#include <nanvix/hlib.h>
#include <posix/stdint.h>

/**
 * @brief Control the timer frequency.
 */
PRIVATE uint32_t arm64_frequency = 0;

/**
 * @brief Was the timer initialized?
 */
PRIVATE bool initialized = false;

/**
 * @brief Read the counter-timer frequency register.
 *
 * @return Counter-timer frequency.
 */
PRIVATE uint32_t arm64_timer_read_cntfrq_el0(void)
{
	uint32_t cntfrq_el0;

	__asm__ __volatile__("mrs %0, CNTFRQ_EL0\n\t" : "=r" (cntfrq_el0) : : "memory");
	return cntfrq_el0;
}

/**
 * @brief Read the counter-timer virtual register.
 * 
 * @return Virtual count value.
 */
PRIVATE uint64_t arm64_timer_read_cntvct_el0(void)
{
	uint64_t cntvct_el0;

	__asm__ __volatile__("mrs %0, CNTVCT_EL0\n\t" : "=r" (cntvct_el0) : : "memory");
	return cntvct_el0;
}

/**
 * @brief Write the counter-time virtual compare value
 *
 * @param cntv_cval_el0 Value to compare.
 */
PRIVATE void arm64_timer_write_cntv_cval_el0(uint64_t cntv_cval_el0)
{
	__asm__ __volatile__("msr CNTV_CVAL_EL0, %0\n\t" : : "r" (cntv_cval_el0) : "memory");
}

/**
 * @brief Read the counter-time virtual control register.
 *
 * @return counter-time virtual controler register.
 */
PRIVATE uint32_t arm64_timer_read_cntv_ctl(void)
{
	uint32_t cntv_ctl;

	__asm__ __volatile__("mrs %0, CNTV_CTL_EL0\n\t" : "=r" (cntv_ctl) :  : "memory");
	return cntv_ctl;
}

/**
 * @brief Disable the timer
 */
PRIVATE void arm64_timer_disable_cntv(void)
{
	uint32_t cntv_ctl;

	cntv_ctl = arm64_timer_read_cntv_ctl();
	cntv_ctl &= ~ARM64_TIMER_CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

/**
 * @brief Enable the timer
 */
PRIVATE void arm64_timer_enable_cntv(void)
{
	uint32_t cntv_ctl;

	cntv_ctl = arm64_timer_read_cntv_ctl();
	cntv_ctl |= ARM64_TIMER_CNTV_CTL_ENABLE;
	__asm__ __volatile__("msr CNTV_CTL_EL0, %0\n\t" : : "r" (cntv_ctl) : "memory");
}

/**
 * @brief Reset the timer counter
 * 
 * @note You must call this function to enable the next timer interruption.
 */
PUBLIC void arm64_timer_reset(void)
{
	if (!initialized)
		return;
	arm64_gic_eoi(TIMER_IRQ);
	arm64_timer_disable_cntv();
	arm64_gicd_clear_pending(TIMER_IRQ);
	uint32_t cntfrq = arm64_timer_read_cntfrq_el0();
	uint64_t ticks = cntfrq/arm64_frequency;
	uint64_t current_cnt = arm64_timer_read_cntvct_el0();
	arm64_timer_write_cntv_cval_el0(current_cnt + ticks);
	arm64_timer_enable_cntv();
	arm64_enable_irq();
}

/**
 * @brief Configure and enable the timer unit.
 * 
 * @param freq Timer frequency (Hz).
 * 
 */
PUBLIC void arm64_timer_init(uint64_t freq)
{
	arm64_gic_configure_timer();
	arm64_timer_disable_cntv();
	arm64_gicd_clear_pending(TIMER_IRQ);
	uint32_t cntfrq = arm64_timer_read_cntfrq_el0();
	uint64_t ticks = cntfrq/freq;
	uint64_t current_cnt = arm64_timer_read_cntvct_el0();
	arm64_timer_write_cntv_cval_el0(current_cnt + ticks);
	arm64_timer_enable_cntv();
	initialized = true;
	arm64_frequency = freq;
}
