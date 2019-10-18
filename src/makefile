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
# Conflicts
#===============================================================================

.PHONY: hal
.PHONY: test

#===============================================================================

# Builds Everything
all: all-hal all-test

# Cleans Build Objects
clean: clean-hal clean-test

# Cleans Everything
distclean: distclean-hal distclean-test

#===============================================================================
# test 
#===============================================================================

# Builds The Test Driver
all-test: all-hal
	$(MAKE) -C test all

# Cleans Test Driver Build Objects
clean-test:
	$(MAKE) -C test clean

# Cleans Test Driver Build
distclean-test:
	$(MAKE) -C test distclean

#===============================================================================
# hal
#===============================================================================

# Builds the Kernel
all-hal:
	$(MAKE) -C hal all

# Cleans Kernel Build Objects
clean-hal:
	$(MAKE) -C hal clean

# Cleans Kernel Build
distclean-hal:
	$(MAKE) -C hal distclean
