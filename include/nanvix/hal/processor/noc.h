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

#ifndef HAL_PROCESSOR_NOC_H_
#define HAL_PROCESSOR_NOC_H_

	/* Processor Interface Implementation */
	#include <nanvix/hal/processor/_processor.h>

	#include <nanvix/klib.h>

/*============================================================================*
 * Interface Implementation Checking                                          *
 *============================================================================*/

#ifdef __INTERFACE_CHECK

	/* Feature Checking */
	#ifndef PROCESSOR_HAS_NOC
	#error "Does this processor have Network-on-Chip (NoC) ?"
	#endif

	/* Multicluster Processor*/
	#if (PROCESSOR_HAS_NOC)

		/* Constants */
		#ifndef HAL_NR_NOC_IONODES
		#error "HAL_NR_NOC_IONODES not defined"
		#endif
		#ifndef HAL_NR_NOC_CNODES
		#error "HAL_NR_NOC_CNODES not defined"
		#endif
		#ifndef HAL_NR_NOC_NODES
		#error "HAL_NR_NOC_NODES not defined"
		#endif

		/* Functions */
		#ifndef __processor_noc_is_ionode0_fn
		#error "__processor_noc_is_ionode0() not defined?"
		#endif
		#ifndef __processor_noc_is_ionode1_fn
		#error "__processor_noc_is_ionode1() not defined?"
		#endif
		#ifndef __processor_noc_is_ionode_fn
		#error "__processor_noc_is_ionode() not defined?"
		#endif
		#ifndef __processor_noc_is_cnode_fn
		#error "__processor_noc_is_cnode() not defined?"
		#endif
		#ifndef __processor_node_get_id_fn
		#error "__processor_node_get_id() not defined?"
		#endif
		#ifndef __processor_node_get_num_fn
		#error "__processor_node_get_num() not defined?"
		#endif
		#ifndef __processor_nodes_convert_fn
		#error "__processor_nodes_convert() not defined?"
		#endif

	#else

		/* Constants */
		#define HAL_NR_NOC_IONODES 1
		#define HAL_NR_NOC_CNODES 0
		#define HAL_NR_NOC_NODES (HAL_NR_NOC_IONODES + HAL_NR_NOC_CNODES)

	#endif

#endif

/*============================================================================*
 * NoC Interface                                                              *
 *============================================================================*/

/**
 * @defgroup kernel-hal-processor-noc Network-on-Chip
 * @ingroup kernel-hal-processor
 *
 * @brief Processor NoC HAL Interface
 */
/**@{*/

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 0.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 0,
	 * and zero otherwise.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_noc_is_ionode0(int nodeid);
#else
	static inline int processor_noc_is_ionode0(int nodeid)
	{
		UNUSED(nodeid);

		return (1);
	}
#endif

	/**
	 * @brief Asserts whether a NoC node is attached to IO cluster 1.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to IO cluster 1,
	 * and zero otherwise.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_noc_is_ionode1(int nodeid);
#else
	static inline int processor_noc_is_ionode1(int nodeid)
	{
		UNUSED(nodeid);

		return (1);
	}
#endif

	/**
	 * @brief Asserts whether a NoC node is attached to an IO cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to an IO cluster,
	 * and zero otherwise.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_noc_is_ionode(int nodeid);
#else
	static inline int processor_noc_is_ionode(int nodeid)
	{
		UNUSED(nodeid);

		return (1);
	}
#endif

	/**
	 * @brief Asserts whether a NoC node is attached to a compute cluster.
	 *
	 * @param nodeid ID of the target NoC node.
	 *
	 * @returns One if the target NoC node is attached to a compute
	 * cluster, and zero otherwise.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_noc_is_cnode(int nodeid);
#else
	static inline int processor_noc_is_cnode(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}
#endif

	/**
	 * @brief Gets the ID of the NoC node attached to the underlying core.
	 *
	 * @returns The ID of the NoC node attached to the underlying core is
	 * returned.
	 *
	 * @note This function is blocking.
	 * @note This function is thread-safe.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_node_get_id(void);
#else
	static inline int processor_node_get_id(void)
	{
		return (0);
	}
#endif

	/**
	 * @brief Gets the logic number of the target NoC node.
	 *
	 * @param nodeid ID of the target NoC node.
	 * @returns The logic number of the target NoC node.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_node_get_num(int nodeid);
#else
	static inline int processor_node_get_num(int nodeid)
	{
		UNUSED(nodeid);

		return (0);
	}
#endif

	/**
	 * @brief Converts a nodes list.
	 *
	 * @param _nodes Place to store converted list.
	 * @param nodes  Target nodes list.
	 * @param nnodes Number of nodes in the list.
	 *
	 * @returns Upon successful completion, zero is returned. Upon
	 * failure, a negative error code is returned instead.
	 */
#if (PROCESSOR_HAS_NOC)
	EXTERN int processor_nodes_convert(int *_nodes, const int *nodes, int nnodes);
#else
	static inline int processor_nodes_convert(int *_nodes, const int *nodes, int nnodes)
	{
		kmemcpy(_nodes, nodes, nnodes);

		return (0);
	}
#endif

/**@}*/

#endif /* HAL_PROCESSOR_NOC_H_ */
