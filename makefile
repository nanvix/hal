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
# Build Options
#===============================================================================

# Verbose build?
export VERBOSE ?= no

# Run Arguments
export RUN_ARGS ?= "--all"

#===============================================================================
# Directories
#===============================================================================

# Directories
export BINDIR    := $(CURDIR)/bin
export BUILDDIR  := $(CURDIR)/build
export LINKERDIR := $(BUILDDIR)/$(TARGET)/linker
export MAKEDIR   := $(BUILDDIR)/$(TARGET)/make
export DOCDIR    := $(CURDIR)/doc
export INCDIR    := $(CURDIR)/include
export LIBDIR    := $(CURDIR)/lib
export SRCDIR    := $(CURDIR)/src
export TOOLSDIR  := $(CURDIR)/tools

#===============================================================================

# Target-Specific Make Rules
include $(MAKEDIR)/makefile

#===============================================================================

export ARTIFACTS := $(shell find $(INCDIR) -name *.h -type f)

#===============================================================================

# Binary Name
export EXECBIN = test-driver

# Image Name
export IMAGE = hal-debug.img

# Builds everything.
all: image

# Runs Unit Tests in all clusters
run:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) all --no-debug $(RUN_ARGS)

# Runs Unit Tests in IO Cluster.
run-iocluster:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) iocluster --no-debug $(RUN_ARGS)

# Runs Unit Tests in Compute Cluster.
run-ccluster:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) ccluster --no-debug $(RUN_ARGS)

# Runs Unit Tests in all clusters in debug mode.
debug:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) all --debug $(RUN_ARGS)

# Runs Unit Tests in IO Cluster in debug mode.
debug-iocluster:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) iocluster --debug $(RUN_ARGS)

# Runs Unit Tests in Compute Cluster in debug mode.
debug-ccluster:
	@bash $(TOOLSDIR)/utils/nanvix-run.sh $(IMAGE) $(BINDIR)/$(EXECBIN) $(TARGET) ccluster --debug $(RUN_ARGS)

# Builds image.
image: hal-target
	@bash $(TOOLSDIR)/image/build-image.sh $(BINDIR) $(IMAGE)

# Make directories
make-dirs:
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)

# Builds HAL.
hal-target: make-dirs
	@$(MAKE) -C $(SRCDIR) -f build/processor/makefile.$(PROCESSOR) all

# Cleans everything.
distclean: distclean-target
	@rm -rf $(IMAGE) $(BINDIR)/$(EXECBIN)
	@rm -rf $(BINDIR) $(LIBDIR)
	@find $(SRCDIR) -name "*.o" -exec rm -rf {} \;

# Cleans compilation files.
distclean-target:
	@$(MAKE) -C $(SRCDIR) -f build/processor/makefile.$(PROCESSOR) distclean

# Install
install: | hal-target copy-artifacts
	@mkdir -p $(PREFIX)/lib
	@cp -f $(LIBDIR)/libhal*.a $(PREFIX)/lib
	@echo [CP] $(LIBDIR)/libhal*.a
	@echo "==============================================================================="
	@echo "Nanvix HAL Successfully Installed into $(PREFIX)"
	@echo "==============================================================================="

# Uninstall
uninstall: | distclean delete-artifacts
	@rm -f $(PREFIX)/lib/libhal*.a
	@echo [RM] $(PREFIX)/lib/libhal*.a
	@echo "==============================================================================="
	@echo "Nanvix HAL Successfully Uninstalled from $(PREFIX)"
	@echo "==============================================================================="

# Copies All Artifacts
copy-artifacts: $(patsubst $(CURDIR)/%, copy/%, $(ARTIFACTS))

# Copy a Single Artifact
copy/%: %
	$(eval file := $(<F))
	$(eval dir := $(<D))
	@echo [CP] $(dir)/$(file)
	@mkdir -p $(PREFIX)/$(dir)
	@cp -f $< $(PREFIX)/$(dir)/$(file)
	@chmod 444 $(PREFIX)/$(dir)/$(file)

# Deletes All Artifacts
delete-artifacts: $(patsubst $(CURDIR)/%, delete/%, $(ARTIFACTS))

# Deletes a Single Artifact
delete/%:
	$(eval file := $(patsubst delete/%, %, $@))
	@echo [RM] $(file)
	@rm -f $(PREFIX)/$(file)

# Builds documentation.
documentation:
	mkdir -p $(DOCDIR)
	doxygen doxygen/doxygen.$(TARGET)
