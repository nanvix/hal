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

#ifndef ARCH_CORE_K1B_MOS_H_
#define ARCH_CORE_K1B_MOS_H_

/**
 * @addtogroup k1b-core-mos mOS
 * @ingroup k1b-core
 *
 * @brief Kalray mOS Hypervisor
 */
/**@{*/

	#include <mOS_common_types_s_c.h>

#ifndef _ASM_FILE_

	#include <HAL/hal/board/boot_args.h>
	#include <HAL/hal/core/diagnostic.h>
	#include <HAL/hal/core/legacy.h>
	#include <HAL/hal/cluster/dsu.h>
	#include "HAL/hal/cluster/cnoc.h"
	#include "HAL/hal/cluster/dnoc.h"
	#include <HAL/hal/hal_ext.h>
	#include <mOS_segment_manager_u.h>
	#include <mOS_common_types_c.h>
	#include "mOS_constants_c.h"
	#include "mOS_scoreboard_c.h"
	#include "mOS_domain_u.h"
	#include <mOS_vcore_u.h>
	#include <mOS_mailbox_c.h>
	#include <mOS_mailbox_u.h>
	#include "mOS_dnoc_c.h"
	#include "mOS_dnoc_u.h"
	#include "mOS_shaper_c.h"
	#include "mOS_shaper_u.h"
	#include "mOS_ucore_c.h"
	#include "mOS_ucore_u.h"
	#include "mppa_bsp.h"
	#include "mOS_bugs_c.h"
	#include <vbsp.h>

#endif /* !_ASM_FILE_ */

/**@}*/

#endif /* ARCH_CORE_K1B_MOS_H_ */
