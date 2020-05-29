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
	 * @brief Number of NoC nodes attached to an IO cluster.
	 */
	#define LINUX64_PROCESSOR_NOC_IONODES_NUM 4

	/**
	 * @brief Number of NoC nodes not attached to an compute cluster.
	 */
	#define LINUX64_PROCESSOR_NOC_CNODES_NUM 8

	/**
	 * @brief Logical NoC node ID of master.
	 */
	#define LINUX64_PROCESSOR_NODENUM_MASTER 0

	/**
	 * @brief Logical NoC node ID of leader.
	 */
	#define LINUX64_PROCESSOR_NODENUM_LEADER 0

#ifdef __NANVIX_HAL

	/**
	 * @brief Powers on the network-on-chip.
	 */
	EXTERN void linux64_processor_noc_boot(void);

	/**
	 * @brief Powers off the network-on-chip.
	 */
	EXTERN void linux64_processor_noc_shutdown(void);

#endif /* __NANVIX_HAL */

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
	#define PROCESSOR_NOC_IONODES_NUM LINUX64_PROCESSOR_NOC_IONODES_NUM /**< LINUX64_PROCESSOR_NOC_IONODES_NUM */
	#define PROCESSOR_NOC_CNODES_NUM  LINUX64_PROCESSOR_NOC_CNODES_NUM  /**< LINUX64_PROCESSOR_NOC_CNODES_NUM  */
	#define PROCESSOR_NODENUM_MASTER  LINUX64_PROCESSOR_NODENUM_MASTER  /**< LINUX64_PROCESSOR_NODENUM_MASTER  */
	#define PROCESSOR_NODENUM_LEADER  LINUX64_PROCESSOR_NODENUM_LEADER  /**< LINUX64_PROCESSOR_NODENUM_LEADER  */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __processor_noc_setup_fn     /**< processor_noc_setup()     */
	#define __processor_noc_is_ionode_fn /**< processor_noc_is_ionode() */
	#define __processor_noc_is_cnode_fn  /**< processor_noc_is_cnode()  */
	#define __processor_node_get_num_fn  /**< processor_node_get_num()  */
	/**@}*/

	/**
	 * @brief Initializes the noc interface.
	 */
	static inline void processor_noc_setup(void)
	{

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

	/**
	 * @brief Gets the logic number of the target NoC node
	 * attached with a core.
	 *
	 * @returns The logic number of the target NoC node attached
	 * with the @p coreid.
	 */
	static inline int processor_node_get_num(void)
	{
		return (linux64_cluster_get_num());
	}

/**@endcond*/

#endif /* PROCESSOR_LINUX64_PROCESSOR_NOC_H_ */


