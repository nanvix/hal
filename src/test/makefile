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
# Toolchain Configuration
#===============================================================================

# Stress Tests Flag
ifeq ($(__ENABLE_STRESS_TESTS),yes)
  CFLAGS += -D__ENABLE_STRESS_TESTS
endif

# Libraries
LIBS := -Wl,--whole-archive
LIBS += $(LIBDIR)/$(LIBHAL)
LIBS += -Wl,--no-whole-archive
LIBS += $(LIBDIR)/$(BARELIB) $(THEIR_LIBS)

#===============================================================================
# Sources
#===============================================================================

# Source Files
C_SRC = $(wildcard *.c)           \
        $(wildcard core/*.c)      \
        $(wildcard cluster/*.c)   \
        $(wildcard processor/*.c) \
        $(wildcard target/*.c)

#===============================================================================
# Sources (Target Specific)
#===============================================================================

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
C_SRC += core/core.c
C_SRC += core/mmu.c
C_SRC += core/spinlock.c
C_SRC += core/tlb.c
C_SRC += core/syscalls.c
C_SRC += core/exception.c
C_SRC += core/interrupt.c
C_SRC += core/perf.c

endif

#===============================================================================
# Object Files
#===============================================================================

OBJ = $(ASM_SRC:.S=.$(OBJ_SUFFIX).o) \
	  $(C_SRC:.c=.$(OBJ_SUFFIX).o)

#===============================================================================

ifeq ($(TARGET),unix64)
LINKER_SCRIPT=
else
LINKER_SCRIPT = -L $(LINKERDIR)/ -T link.ld
endif

# Builds All Object Files
all: $(OBJ)
ifeq ($(VERBOSE), no)
	@echo [CC] $(BINDIR)/$(EXEC)
	@$(CC) $(LDFLAGS) $(LINKER_SCRIPT) -o $(BINDIR)/$(EXEC) $(OBJ) $(LIBS)
else
	$(CC) $(LDFLAGS) $(LINKER_SCRIPT) -o $(BINDIR)/$(EXEC) $(OBJ) $(LIBS)
endif

# Cleans All Object Files
clean:
ifeq ($(VERBOSE), no)
	@echo [CLEAN] $(OBJ)
	@rm -rf $(OBJ)
else
	rm -rf $(OBJ)
endif

# Cleans Everything
distclean: clean
ifeq ($(VERBOSE), no)
	@echo [CLEAN] $(BINDIR)/$(EXEC)
	@rm -rf $(BINDIR)/$(EXEC)
else
	rm -rf $(BINDIR)/$(EXEC)
endif

# builds a C source file.
%.$(OBJ_SUFFIX).o: %.c
ifeq ($(VERBOSE), no)
	@echo [CC] $@
	@$(CC) $(CFLAGS) $< -c -o $@
else
	$(CC) $(CFLAGS) $< -c -o $@
endif

# Builds an ASM source file.
%.$(OBJ_SUFFIX).o: %.S
ifeq ($(VERBOSE), no)
	@echo [CC] $@
	@$(CC) $(CFLAGS) $< -c -o $@
else
	$(CC) $(CFLAGS) $< -c -o $@
endif
