#
# MIT License
#
# Copyright(c) 2011-2020 The Maintainers of Nanvix
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

include $(BUILDDIR)/makefile.config

#===============================================================================
# Binaries Sources and Objects
#===============================================================================

# Stress Tests Flag
ifeq ($(__ENABLE_STRESS_TESTS),yes)
  CFLAGS += -D__ENABLE_STRESS_TESTS
endif

# Binary
EXEC = hal-tests.$(OBJ_SUFFIX)

# Source Files
C_SRC = $(wildcard *.c)           \
        $(wildcard core/*.c)      \
        $(wildcard cluster/*.c)   \
        $(wildcard processor/*.c) \
        $(wildcard target/*.c)    \
        $(wildcard stress/*.c)    \
        $(wildcard abstract/*.c)  \
        $(wildcard utils/*.c)

# Sources (Target Specific)
ifeq ($(TARGET), mppa256)

ASM_SRC = $(wildcard core/arch/k1b.S)

else ifeq ($(TARGET), optimsoc)

ASM_SRC = $(wildcard core/arch/or1k.S)

else ifeq ($(TARGET), qemu-openrisc)

ASM_SRC = $(wildcard core/arch/or1k.S)

else ifeq ($(TARGET), qemu-riscv32)

ASM_SRC = $(wildcard core/arch/rv32gc.S)

else ifeq ($(TARGET), qemu-x86)

ASM_SRC = $(wildcard core/arch/i486.S)

else ifeq ($(TARGET), unix64)

C_SRC := $(filter-out $(wildcard core/*.c), $(C_SRC))
C_SRC += core/arithmetic.c
C_SRC += core/core.c
C_SRC += core/mmu.c
C_SRC += core/spinlock.c
C_SRC += core/tlb.c
C_SRC += core/syscalls.c
C_SRC += core/exception.c
C_SRC += core/interrupt.c
C_SRC += core/perf.c

else ifeq ($(TARGET), qemu-arm64)

C_SRC = 

endif

# Object Files
OBJ = $(ASM_SRC:.S=.$(OBJ_SUFFIX).o) \
      $(C_SRC:.c=.$(OBJ_SUFFIX).o)

#===============================================================================

include $(BUILDDIR)/makefile.executable
