#
# MIT License
#
# Copyright(c) 2011-2019 The Maintainers of Nanvix
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

#===============================================================================
# Target Configuration
#===============================================================================

export TARGET    ?= qemu-openrisc
export PROCESSOR := or1k

#===============================================================================
# Toolchain Configuration
#===============================================================================

# Toolchain Directory
export TOOLCHAIN_DIR ?= $(TOOLSDIR)/dev/toolchain/or1k/bin/

# Toolchain
export CC = $(TOOLCHAIN_DIR)/or1k-elf-gcc
export LD = $(TOOLCHAIN_DIR)/or1k-elf-ld
export AR = $(TOOLCHAIN_DIR)/or1k-elf-ar

# Timeout Variable
export TIMEOUT ?= 5

# Compiler Options
export CFLAGS  = -D __qemu_openrisc__ -D__openrisc__
export CFLAGS += -nostdlib -nostdinc
export CFLAGS += -ansi -pedantic-errors
export CFLAGS += -Wstack-usage=4096
export CFLAGS += -Wno-type-limits
export CFLAGS += -D __HAS_HW_DIVISION
export CFLAGS += -D __INTERFACE_CHECK_CORE_AL
export CFLAGS += -D __INTERFACE_CHECK_EVENT
export CFLAGS += -D __INTERFACE_CHECK_MEMORY
export CFLAGS += -D __INTERFACE_CHECK_PROCESSOR_AL
export CFLAGS += -D __INTERFACE_CHECK_TARGET_AL
export CFLAGS += -I $(INCDIR)/posix

# Linker Options
export LDFLAGS  =

#===============================================================================
# Libraries and Binaries
#===============================================================================

# Libraries
export LIBHAL = libhal-$(TARGET).a
export LIBS = $(LIBDIR)/$(LIBHAL)
