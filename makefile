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

#===============================================================================
# Directories
#===============================================================================

# Directories
export BINDIR    = $(CURDIR)/bin
export BUILDDIR  = $(CURDIR)/build
export LINKERDIR = $(BUILDDIR)/$(TARGET)/linker
export MAKEDIR   = $(BUILDDIR)/$(TARGET)/make
export DOCDIR    = $(CURDIR)/doc
export INCDIR    = $(CURDIR)/include
export LIBDIR    = $(CURDIR)/lib
export SRCDIR    = $(CURDIR)/src
export TOOLSDIR  = $(CURDIR)/tools

#===============================================================================

# Target-Specific Make Rules
include $(MAKEDIR)/makefile

#===============================================================================

export ARTIFACTS =  include/arch/
export ARTIFACTS += include/arch/stdout/8250.h
export ARTIFACTS += include/arch/stdout/console.h
export ARTIFACTS += include/arch/stdout/jtag.h
export ARTIFACTS += include/grub/mboot.h
export ARTIFACTS += include/machine/_default_types.h
export ARTIFACTS += include/nanvix/hal/
export ARTIFACTS += include/nanvix/const.h
export ARTIFACTS += include/nanvix/klib.h
export ARTIFACTS += include/nanvix/hal/log.h
export ARTIFACTS += include/sys/_intsup.h
export ARTIFACTS += include/sys/features.h
export ARTIFACTS += include/sys/types.h
export ARTIFACTS += include/decl.h
export ARTIFACTS += include/errno.h
export ARTIFACTS += include/stdarg.h
export ARTIFACTS += include/stddef.h
export ARTIFACTS += include/stdint.h

#===============================================================================

# Binary Name
export EXECBIN = test-driver

# Image Name
export IMAGE = hal-debug.img

# Builds everything.
all: image

# Builds image.
image: | hal hal-target
	bash $(TOOLSDIR)/image/build-image.sh $(BINDIR) $(IMAGE)

# Builds Nanvix.
hal:
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)

# Builds HAL.
hal-target:
	@$(MAKE) -C $(SRCDIR) -f build/processor/makefile.$(PROCESSOR) all

# Cleans everything.
distclean: distclean-target
	@rm -rf $(IMAGE)
	@rm -rf $(BINDIR) $(LIBDIR)
	@find $(SRCDIR) -name "*.o" -exec rm -rf {} \;	

# Cleans compilation files.
distclean-target:
	@$(MAKE) -C $(SRCDIR) -f build/processor/makefile.$(PROCESSOR) distclean

# Install
install: $(ARTIFACTS) | hal hal-target
	mkdir -p $(PREFIX)/include/
	cp -r include/arch/                    $(PREFIX)/include/arch/
	mkdir -p $(PREFIX)/include/grub/
	cp -r include/grub/mboot.h             $(PREFIX)/include/grub/
	mkdir -p $(PREFIX)/include/machine/
	cp -r include/machine/_default_types.h $(PREFIX)/include/machine/
	mkdir -p $(PREFIX)/include/nanvix/
	cp -r include/nanvix/hal/              $(PREFIX)/include/nanvix/
	cp -r include/nanvix/hal/log.h $(PREFIX)/include/nanvix/hal/
	cp -r include/nanvix/const.h $(PREFIX)/include/nanvix/
	cp -r include/nanvix/klib.h $(PREFIX)/include/nanvix/
	mkdir -p $(PREFIX)/include/sys/
	cp -r include/sys/_intsup.h $(PREFIX)/include/sys/
	cp -r include/sys/features.h $(PREFIX)/include/sys/
	cp -r include/sys/types.h $(PREFIX)/include/sys/
	cp -r include/decl.h $(PREFIX)/include/
	cp -r include/errno.h $(PREFIX)/include/
	cp -r include/stdarg.h $(PREFIX)/include/
	cp -r include/stddef.h $(PREFIX)/include/
	cp -r include/stdint.h $(PREFIX)/include/
	cp -r lib/ $(PREFIX)/

# Uninstall
uninstall:
	cd $(PREFIX) && rm -rf $(ARTIFACTS)

# Builds documentation.
documentation:
	mkdir -p $(DOCDIR)
	doxygen doxygen/doxygen.$(TARGET)
