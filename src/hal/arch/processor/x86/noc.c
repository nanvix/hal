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

#define __NEED_HAL_PROCESSOR

#include <nanvix/hal/processor.h>
#include <nanvix/const.h>

/*============================================================================*
 * x86_processor_noc_setup()                                                  *
 *============================================================================*/

/**
 * @brief Initializes the noc interface.
 */
PUBLIC void x86_processor_noc_setup(void)
{

}

/*============================================================================*
 * x86_processor_noc_is_ionode()                                              *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC int x86_processor_noc_is_ionode(int nodenum)
{
	UNUSED(nodenum);

	return (1);
}

/*============================================================================*
 * x86_processor_noc_is_cnode()                                               *
 *============================================================================*/

/**
 * @todo TODO: Provide a detailed description to this function.
 */
PUBLIC int x86_processor_noc_is_cnode(int nodenum)
{
	UNUSED(nodenum);

	return (0);
}

/*============================================================================*
 * x86_processor_node_get_num()                                               *
 *============================================================================*/

/**
 * @brief Gets the logic number of the target NoC node
 * attached with a core.
 *
 * @param coreid Attached core ID.
 *
 * @returns The logic number of the target NoC node attached
 * with the @p coreid.
 */
PUBLIC int x86_processor_node_get_num(int coreid)
{
	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	return (0);
}

/*============================================================================*
 * x86_processor_node_set_num()                                               *
 *============================================================================*/

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
PUBLIC int x86_processor_node_set_num(int coreid, int nodenum)
{
	/* Invalid coreid. */
	if (!WITHIN(coreid, 0, CORES_NUM))
		return (-EINVAL);

	/* Invalid coreid. */
	if (!WITHIN(nodenum, 0, PROCESSOR_NOC_NODES_NUM))
		return (-EINVAL);

	return (0);
}
