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

#ifndef CLUSTER_X86_CLUSTER_MEMORY_H_
#define CLUSTER_X86_CLUSTER_MEMORY_H_

	/* Cluster Interface Implementation */
	#include <arch/cluster/x86-cluster/_x86-cluster.h>

/**
 * @addtogroup x86-cluster-mem Memory
 * @ingroup x86-cluster
 *
 * @brief Memory System
 */
/**@{*/

	/* Must come first. */
	#define __NEED_CLUSTER_MEMMAP

	#include <nanvix/const.h>
	#include <arch/cluster/x86-cluster/memmap.h>

	/**
	 * @name Physical Memory Layout
	 */
	/**@{*/
	#define X86_CLUSTER_KERNEL_BASE_PHYS X86_CLUSTER_DRAM_BASE_PHYS                       /**< Kernel Code and Data */
	#define X86_CLUSTER_KERNEL_END_PHYS  (X86_CLUSTER_KERNEL_BASE_PHYS + I486_PGTAB_SIZE) /**< Kernel End           */
	#define X86_CLUSTER_KPOOL_BASE_PHYS  (X86_CLUSTER_KERNEL_END_PHYS  + I486_PGTAB_SIZE) /**< Kernel Page Pool     */
	#define X86_CLUSTER_KPOOL_END_PHYS   (X86_CLUSTER_KPOOL_BASE_PHYS  + I486_PGTAB_SIZE) /**< Kernel Pool End      */
	#define X86_CLUSTER_USER_BASE_PHYS   X86_CLUSTER_KPOOL_END_PHYS                       /**< User Base            */
	#define X86_CLUSTER_USER_END_PHYS    X86_CLUSTER_DRAM_END_PHYS                        /**< User End             */
	/**@}*/

	/**
	 * @name Virtual Memory Layout
	 */
	/**@{*/
	#define X86_CLUSTER_KERNEL_BASE_VIRT X86_CLUSTER_KERNEL_BASE_PHYS  /**< Kernel Code and Data */
	#define X86_CLUSTER_KERNEL_END_VIRT  X86_CLUSTER_KERNEL_END_PHYS   /**< Kernel End           */
	#define X86_CLUSTER_KPOOL_BASE_VIRT  X86_CLUSTER_KPOOL_BASE_PHYS   /**< Kernel Page Pool     */
	#define X86_CLUSTER_KPOOL_END_VIRT   X86_CLUSTER_KPOOL_END_PHYS    /**< Kernel Pool End      */
	#define X86_CLUSTER_USER_BASE_VIRT   0xa0000000                    /**< User Base            */
	#define X86_CLUSTER_USER_END_VIRT    0xc0000000                    /**< User End             */
	#define X86_CLUSTER_USTACK_BASE_VIRT 0xc0000000                    /**< User Stack Base      */
	#define X86_CLUSTER_USTACK_END_VIRT  0xb0000000                    /**< User Stack End       */
	/**@}*/

	/**
	 * @name Memory Regions Constants
	 */
	/**@{*/
	#define X86_CLUSTER_MEM_REGIONS            2                        /**< Memory Regions number.            */
	#define X86_CLUSTER_ROOT_PGTAB_NUM         X86_CLUSTER_MEM_REGIONS  /**< Root page table size.             */
	#define X86_CLUSTER_MREGION_PT_ALIGN_START 0                        /**< MRegion start page table aligned. */
	#define X86_CLUSTER_MREGION_PT_ALIGN_END   2                        /**< MRegion end page table aligned.   */
	#define X86_CLUSTER_MREGION_PG_ALIGN_START 0                        /**< MRegion start page aligned.       */
	#define X86_CLUSTER_MREGION_PG_ALIGN_END   0                        /**< MRegion end page aligned.         */
	/**@}*/

	/**
	 * @brief Memory size (in bytes).
	 */
	#define X86_CLUSTER_MEM_SIZE \
		X86_CLUSTER_DRAM_SIZE

	/**
	 * @brief Kernel memory size (in bytes).
	 */
	#define X86_CLUSTER_KMEM_SIZE \
		(X86_CLUSTER_KERNEL_END_PHYS - X86_CLUSTER_KERNEL_BASE_PHYS)

	/**
	 * @brief Kernel page pool size (in bytes).
	 */
	#define X86_CLUSTER_KPOOL_SIZE \
		(X86_CLUSTER_KPOOL_END_PHYS - X86_CLUSTER_KPOOL_BASE_PHYS)

	/**
	 * @brief User memory size (in bytes).
	 */
	#define X86_CLUSTER_UMEM_SIZE \
		(X86_CLUSTER_USER_END_PHYS - X86_CLUSTER_USER_BASE_PHYS)

#ifndef _ASM_FILE

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

#ifdef __NANVIX_HAL

	/**
	 * @brief Enable paging in the underlying core.
	 */
	EXTERN void _x86_cluster_enable_paging(void);

#endif /* __NANVIX_HAL */

#endif /* _ASM_FILE_ */

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond x86_cluster
 */

	/**
	 * @name Exported Constants
	 */
	#define MEMORY_SIZE X86_CLUSTER_MEM_SIZE         /**< @see X86_CLUSTER_MEM_SIZE         */
	#define KMEM_SIZE   X86_CLUSTER_KMEM_SIZE        /**< @see X86_CLUSTER_KMEM_SIZE        */
	#define UMEM_SIZE   X86_CLUSTER_UMEM_SIZE        /**< @see X86_CLUSTER_UMEM_SIZE        */
	#define KSTACK_SIZE X86_CLUSTER_KSTACK_SIZE      /**< @see X86_CLUSTER_KSTACK_SIZE      */
	#define KPOOL_SIZE  X86_CLUSTER_KPOOL_SIZE       /**< @see X86_CLUSTER_KPOOL_SIZE       */
	#define KBASE_PHYS  X86_CLUSTER_KERNEL_BASE_PHYS /**< @see X86_CLUSTER_KERNEL_BASE_PHYS */
	#define KPOOL_PHYS  X86_CLUSTER_KPOOL_BASE_PHYS  /**< @see X86_CLUSTER_KPOOL_BASE_PHYS  */
	#define UBASE_PHYS  X86_CLUSTER_USER_BASE_PHYS   /**< @see X86_CLUSTER_USER_BASE_PHYS   */
	#define USTACK_VIRT X86_CLUSTER_USTACK_BASE_VIRT /**< @see X86_CLUSTER_USTACK_BASE_VIRT */
	#define UBASE_VIRT  X86_CLUSTER_USER_BASE_VIRT   /**< @see X86_CLUSTER_USER_BASE_VIRT   */
	#define UEND_VIRT   X86_CLUSTER_USER_END_VIRT    /**< @see X86_CLUSTER_USER_END_VIRT    */
	#define KBASE_VIRT  X86_CLUSTER_KERNEL_BASE_VIRT /**< @see X86_CLUSTER_KERNEL_BASE_VIRT */
	#define KPOOL_VIRT  X86_CLUSTER_KPOOL_BASE_VIRT  /**< @see X86_CLUSTER_KPOOL_BASE_VIRT  */
	/**@}*/

	/**
	 * @name Exported Memory Region Constants
	 */
	/**@{*/
	#define MEM_REGIONS            X86_CLUSTER_MEM_REGIONS            /**< @see X86_CLUSTER_MEM_REGIONS            */
	#define ROOT_PGTAB_NUM         X86_CLUSTER_ROOT_PGTAB_NUM         /**< @see X86_CLUSTER_ROOT_PGTAB_NUM         */
	#define MREGION_PT_ALIGN_START X86_CLUSTER_MREGION_PT_ALIGN_START /**< @see X86_CLUSTER_MREGION_PT_ALIGN_START */
	#define MREGION_PT_ALIGN_END   X86_CLUSTER_MREGION_PT_ALIGN_END   /**< @see X86_CLUSTER_MREGION_PT_ALIGN_END   */
	#define MREGION_PG_ALIGN_START X86_CLUSTER_MREGION_PG_ALIGN_START /**< @see X86_CLUSTER_MREGION_PG_ALIGN_START */
	#define MREGION_PG_ALIGN_END   X86_CLUSTER_MREGION_PG_ALIGN_END   /**< @see X86_CLUSTER_MREGION_PG_ALIGN_END   */
	/**@}*/

/**@endcond*/

#endif /* CLUSTER_X86_CLUSTER_MEMORY_H_ */
