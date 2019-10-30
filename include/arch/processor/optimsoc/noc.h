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

#ifndef PROCESSOR_OPTIMSOC_PROCESSOR_NOC_H_
#define PROCESSOR_OPTIMSOC_PROCESSOR_NOC_H_

	/* Cluster API. */
	#include <arch/processor/optimsoc/_optimsoc.h>

/**
 * @addtogroup processor-optimsoc_processor-noc NoC
 * @ingroup processor-optimsoc_processor
 *
 * @brief NoC
 */
/**@{*/

	#include <nanvix/const.h>

	/**
	 * @brief Number of NoC nodes attached to an IO cluster.
	 */
	#define OPTIMSOC_PROCESSOR_NOC_IONODES_NUM 1

	/**
	 * @brief Number of NoC nodes not attached to an compute cluster.
	 */
	#define OPTIMSOC_PROCESSOR_NOC_CNODES_NUM 0

	/**
	 * @brief Logical NoC node ID of master.
	 */
	#define OPTIMSOC_PROCESSOR_NODENUM_MASTER 0

	/**
	 * @brief Initializes the noc interface.
	 */
	EXTERN void optimsoc_processor_noc_setup(void);

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodenum Logical number of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
	EXTERN int optimsoc_processor_noc_is_ionode(int nodenum);

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodenum Logical number of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
	EXTERN int optimsoc_processor_noc_is_cnode(int nodenum);

	/**
	 * @brief Gets the logic number of the target NoC node
	 * attached with a core.
	 *
	 * @param coreid Attached core ID.
	 *
	 * @returns The logic number of the target NoC node attached
	 * with the @p coreid.
	 */
	EXTERN int optimsoc_processor_node_get_num(int coreid);

	/**
	 * @brief Exchange the logic number of the target NoC node
	 * attached with a core.
	 *
	 * @param coreid  Attached core ID.
	 * @param nodenum Logic ID of the target NoC node.
	 *
	 * @returns Zero if the target NoC node is successfully attached
	 * to the requested @p coreid, and non zero otherwise.
	 */
	EXTERN int optimsoc_processor_node_set_num(int coreid, int nodenum);

/**@}*/

/*============================================================================*
 * Exported Interface                                                         *
 *============================================================================*/

/**
 * @cond optimsoc_processor
 */

	/**
	 * @name Exported Constans
	 */
	/**@{*/
	#define PROCESSOR_NOC_IONODES_NUM OPTIMSOC_PROCESSOR_NOC_IONODES_NUM /**< OPTIMSOC_PROCESSOR_NOC_IONODES_NUM */
	#define PROCESSOR_NOC_CNODES_NUM  OPTIMSOC_PROCESSOR_NOC_CNODES_NUM  /**< OPTIMSOC_PROCESSOR_NOC_CNODES_NUM  */
	#define PROCESSOR_NODENUM_MASTER  OPTIMSOC_PROCESSOR_NODENUM_MASTER  /**< OPTIMSOC_PROCESSOR_NODENUM_MASTER  */
	/**@}*/

	/**
	 * @name Exported Functions
	 */
	/**@{*/
	#define __processor_noc_setup_fn     /**< processor_noc_setup()     */
	#define __processor_noc_is_ionode_fn /**< processor_noc_is_ionode() */
	#define __processor_noc_is_cnode_fn  /**< processor_noc_is_cnode()  */
	#define __processor_node_get_num_fn  /**< processor_node_get_num()  */
	#define __processor_node_set_num_fn  /**< processor_node_set_num()  */
	/**@}*/

	/**
	 * @brief Dummy operation.
	 */
	static inline void processor_noc_setup(void)
	{
		optimsoc_processor_noc_setup();
	}

	/**
	 * @see optimsoc_processor_noc_is_ionode()
	 */
	static inline int processor_noc_is_ionode(int nodenum)
	{
		return (optimsoc_processor_noc_is_ionode(nodenum));
	}

	/**
	 * @see optimsoc_processor_noc_is_cnode()
	 */
	static inline int processor_noc_is_cnode(int nodenum)
	{
		return (optimsoc_processor_noc_is_cnode(nodenum));
	}

	/**
	 * @see optimsoc_processor_node_get_num().
	 */
	static inline int processor_node_get_num(int coreid)
	{
		return (optimsoc_processor_node_get_num(coreid));
	}

	/**
	 * @see optimsoc_processor_node_set_num().
	 */
	static inline int processor_node_set_num(int coreid, int nodenum)
	{
		return (optimsoc_processor_node_set_num(coreid, nodenum));
	}

/**@endcond*/

#endif /* PROCESSOR_OPTIMSOC_PROCESSOR_NOC_H_ */
