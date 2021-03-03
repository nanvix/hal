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

#include <arch/cluster/arm64-cluster/psci.h>
#include <arch/stdout/pl011.h>

psci_ret psci_cpu_turn_on_print(cpu_id cpuID) {
    uart_pl011_init();
    psci_ret temp = psci_v0_2_cpu_turn_on(cpuID);
    switch (temp)
    {
    case PSCI_RET_SUCCESS:
        uart_pl011_write("PSCI_RET_SUCCESS!\n", 20);
        break;
    case PSCI_RET_INVALID_PARAMS:
        uart_pl011_write("PSCI_RET_INVALID_PARAMS!\n", 20);
        break;
    case PSCI_RET_DENIED:
        uart_pl011_write("PSCI_RET_DENIED!\n", 20);
        break;
    case PSCI_RET_ALREADY_ON:
        uart_pl011_write("PSCI_RET_ALREADY_ON!\n", 22);
        break;
    case PSCI_RET_ON_PENDING:
        uart_pl011_write("PSCI_RET_ON_PENDING!\n", 20);
        break;
    case PSCI_RET_INTERNAL_FAILURE:
        uart_pl011_write("PSCI_RET_INTERNAL_FAILURE!\n", 20);
        break;
    case PSCI_RET_NOT_PRESENT:
        uart_pl011_write("PSCI_RET_NOT_PRESENT!\n", 20);
        break;
    case PSCI_RET_DISABLED:
        uart_pl011_write("PSCI_RET_DISABLED!\n", 20);
        break;
    case PSCI_RET_INVALID_ADDRESS:
        uart_pl011_write("PSCI_RET_INVALID_ADDRESS!\n", 20);
        break;
    default:
        break;
    }
    return temp;
}