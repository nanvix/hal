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

#ifndef ARCH_CLUSTER_WOLFE_MEMORY_H_
#define ARCH_CLUSTER_WOLFE_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/wolfe/_wolfe.h>

/**
 * @addtogroup wolfe-cluster-mem Memory
 * @ingroup wolfe-cluster
 *
 * @brief Memory System
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define WOLFE_PERIPHERALS_PHYS 0x1a100000
	/**@}*/

	/**
	 * @name Offsets to Peripherals
	 */
	/**@{*/
	#define WOLFE_PERIPHERALS_OFF_FLL    0x00000000 /**< Frequency-Locked Loop Unit */
	#define WOLFE_PERIPHERALS_OFF_GPIO   0x00001000 /**< GPIO Unit                  */
	#define WOLFE_PERIPHERALS_OFF_UDMA   0x00002000 /**< Micro-DMA Unit             */
	#define WOLFE_PERIPHERALS_OFF_UART   0x00002000 /**< UART Device                */
	#define WOLFE_PERIPHERALS_OFF_CTRL   0x00004000 /**< Control Unit               */
	#define WOLFE_PERIPHERALS_OFF_PWM    0x00005000 /**< PWM Unit                   */
	#define WOLFE_PERIPHERALS_OFF_EU     0x00006000 /**< Event Unit                 */
	#define WOLFE_PERIPHERALS_OFF_STDOUT 0x0000f000 /**< Standard Output Unit       */
	/**@}*/

	/**
	 * @name Address to Peripherals
	 */
	/**@{*/
	#define WOLFE_FLL_ADDR    (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_FLL)     /**< Frequency-Locked Loop Unit */
	#define WOLFE_GPIO_ADDR   (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_GPIO)    /**< GPIO Unit                  */
	#define WOLFE_UDMA_ADDR   (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_UDMA)    /**< Micro-DMA Unit             */
	#define WOLFE_UART_ADDR   (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_UART)    /**< UART Device                */
	#define WOLFE_CTRL_ADDR   (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_CTRL)    /**< Control Unit               */
	#define WOLFE_PWM_ADDR    (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_PWM)     /**< PWM Unit                   */
	#define WOLFE_EU_ADDR     (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_EU)      /**< Event Unit                 */
	#define WOLFE_STDOUT_ADDR (WOLFE_PERIPHERALS_PHYS + WOLFE_PERIPHERALS_OFF_STDOUT)  /**< Standard Output Unit       */
	/**@}*/

	/**
	 * @broef Core stack size log 2.
	 */
	#define WOLFE_STACK_SIZE_LOG2 10

	/**
	 * @brief Core stack size.
	 */
	#define WOLFE_STACK_SIZE (1 << WOLFE_STACK_SIZE_LOG2)

#ifndef _ASM_FILE_

	/**
	 * @brief Binary Sections
	 */
	/**@{*/
	EXTERN unsigned char __BOOTSTRAP_START; /**< Bootstrap Start */
	EXTERN unsigned char __BOOTSTRAP_END;   /**< Bootstrap End   */
	EXTERN unsigned char __TEXT_START;      /**< Text Start      */
	EXTERN unsigned char __TEXT_END;        /**< Text End        */
	EXTERN unsigned char __DATA_START;      /**< Data Start      */
	EXTERN unsigned char __DATA_END;        /**< Data End        */
	EXTERN unsigned char __BSS_START;       /**< BSS Start       */
	EXTERN unsigned char __BSS_END;         /**< BSS End         */
	/**@}*/

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond wolfe
 */

/**@endcond*/

#endif /* ARCH_CLUSTER_WOLFE_MEMORY_H_ */

