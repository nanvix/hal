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

#===============================================================================
# Sources
#===============================================================================

# C Source Files
C_SRC = $(wildcard *.c)                             \
        $(wildcard arch/core/$(CORE)/*.c)           \
        $(wildcard arch/cluster/$(CLUSTER)/*.c)     \
        $(wildcard arch/processor/$(PROCESSOR)/*.c) \
        $(wildcard arch/target/$(TARGET)/*.c)       \
        $(wildcard core/*.c)                        \
        $(wildcard cluster/*.c)                     \
        $(wildcard hlib/*.c)                        \
        $(wildcard target/*.c)                      \
        $(wildcard stdout/klog.c)

# Asembly Source Files
ASM_SRC = $(wildcard arch/core/$(CORE)/*.S)       \
          $(wildcard arch/cluster/$(CLUSTER)/*.S)

#===============================================================================
# Sources (Target Specific)
#===============================================================================

ifeq ($(TARGET), mppa256)

C_SRC += $(wildcard stdout/jtag.c)

else ifeq ($(TARGET), optimsoc)

C_SRC += $(wildcard arch/core/or1k/*.c)
C_SRC += $(wildcard stdout/systrace.c)
ASM_SRC += $(wildcard arch/core/or1k/*.S)

else ifeq ($(TARGET), qemu-openrisc)

C_SRC += $(wildcard stdout/8250.c)

else ifeq ($(TARGET), qemu-riscv32)

C_SRC += $(wildcard stdout/16550a.c)

else ifeq ($(TARGET), qemu-x86)

C_SRC += $(wildcard stdout/console.c)

else ifeq ($(TARGET), unix64)

# C Source Files
C_SRC += stdout/tty-virt.c

endif

#===============================================================================
# Object Files
#===============================================================================

OBJ = $(ASM_SRC:.S=.$(OBJ_SUFFIX).o) \
	  $(C_SRC:.c=.$(OBJ_SUFFIX).o)

#===============================================================================

# Builds all object files.
all: $(OBJ)
ifeq ($(VERBOSE), no)
	@echo [AR] $(LIBHAL)
	@$(AR) $(ARFLAGS) $(LIBDIR)/$(LIBHAL) $(OBJ)
else
	$(AR) $(ARFLAGS) $(LIBDIR)/$(LIBHAL) $(OBJ)
endif

# Cleans all object files.
clean:
ifeq ($(VERBOSE), no)
	@echo [CLEAN] $(OBJ)
	@rm -rf $(OBJ)
else
	rm -rf $(OBJ)
endif

# Cleans everything.
distclean: clean
ifeq ($(VERBOSE), no)
	@echo [CLEAN] $(LIBHAL)
	@rm -rf $(LIBDIR)/$(LIBHAL)
else
	rm -rf $(LIBDIR)/$(LIBHAL)
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
