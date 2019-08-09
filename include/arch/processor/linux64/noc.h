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

#ifndef PROCESSOR_LINUX64_PROCESSOR_NOC_H_
#define PROCESSOR_LINUX64_PROCESSOR_NOC_H_

	/* Cluster API. */
	#include <arch/processor/linux64/_linux64.h>

/**
 * @addtogroup processor-linux64_processor-noc NoC
 * @ingroup processor-linux64_processor
 *
 * @brief NoC
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @name Number of NoC nodes attached to an IO cluster.
	 */
	#define LINUX64_PROCESSOR_NOC_IONODES_NUM 8

	/**
	 * @name Number of NoC nodes not attached to an compute cluster.
	 */
	#define LINUX64_PROCESSOR_NOC_CNODES_NUM 16

	/**
	 * @name Total number of NoC nodes.
	 */
	#define LINUX64_PROCESSOR_NOC_NODES_NUM \
		(LINUX64_PROCESSOR_NOC_IONODES_NUM + LINUX64_PROCESSOR_NOC_CNODES_NUM)

#ifdef __NANVIX_HAL

	/**
	 * @brief Powers on the network-on-chip.
	 */
	EXTERN void linux64_processor_noc_boot(void);

	/**
	 * @brief Powers off the network-on-chip.
	 */
	EXTERN void linux64_processor_noc_shutdown(void);

	/**
	 * @brief Initializes the NoC.
	 */
	EXTERN void linux64_processor_noc_setup(void);

#endif /* __NANVIX_HAL */

	/**
	 * @brief Gets the ID of the NoC node attached to the underlying core.
	 *
	 * @returns The ID of the NoC node attached to the underlying core is
	 * returned.
	 *
	 * @note This function is blocking.
	 * @note This function is thread-safe.
	 */
	EXTERN int linux64_processor_node_get_id(void);

	/**
	 * @brief Gets the logic number of the target NoC node.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The logic number of the target NoC node.
	 */
	EXTERN int linux64_processor_node_get_num(int nodeid);

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodenum Logical number of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
	EXTERN int linux64_processor_noc_is_ionode(int nodenum);

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodenum Logical number of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
	EXTERN int linux64_processor_noc_is_cnode(int nodenum);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond linux64_processor
 */

	/**
	 * @name Exported Constans
	 */
	/**@{*/
	#define NR_NOC_IONODES LINUX64_PROCESSOR_NOC_IONODES_NUM
	#define NR_NOC_CNODES  LINUX64_PROCESSOR_NOC_CNODES_NUM
	#define NR_NOC_NODES   LINUX64_PROCESSOR_NOC_NODES_NUM
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __processor_node_get_id_fn   /**< processor_node_get_id()   */
	#define __processor_node_get_num_fn   /**< processor_node_get_num()   */
	#define __processor_noc_is_ionode_fn /**< processor_noc_is_ionode()  */
	#define __processor_noc_is_cnode_fn  /**< processor_noc_is_cnode()   */
	/**@}*/

	/**
	 * @see linux64_processor_node_get_id().
	 */
	static inline int processor_node_get_id(void)
	{
		return (linux64_processor_node_get_id());
	}

	/**
	 * @see linux64_processor_node_get_num().
	 */
	static inline int processor_node_get_num(int nodeid)
	{
		return (linux64_processor_node_get_num(nodeid));
	}

	/**
	 * @see linux64_processor_noc_is_ionode()
	 */
	static inline int processor_noc_is_ionode(int nodenum)
	{
		return (linux64_processor_noc_is_ionode(nodenum));
	}

	/**
	 * @see linux64_processor_noc_is_cnode()
	 */
	static inline int processor_noc_is_cnode(int nodenum)
	{
		return (linux64_processor_noc_is_cnode(nodenum));
	}

/**@endcond*/

#endif /* PROCESSOR_LINUX64_PROCESSOR_NOC_H_ */


