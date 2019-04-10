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

# NOTES:
#   - This script should work in any Linux distribution.
#   - You should run this script with superuser privileges.
#

export CURDIR=`pwd`

case "$TARGET" in
	"qemu-x86")
		qemu-system-i386 -s -S      \
			--display curses        \
			-kernel bin/test-driver \
			-m 256M                 \
			-mem-prealloc
		;;
	"qemu-openrisc")
		qemu-system-or1k -s -S      \
			-kernel bin/test-driver \
			-serial stdio           \
			-display none           \
			-m 256M                 \
			-mem-prealloc           \
			-smp 2
		;;
	"qemu-riscv32")
		qemu-system-riscv32 -s -S   \
			-machine virt           \
			-serial stdio           \
			-display none           \
			-kernel bin/test-driver \
			-m 128M                 \
			-mem-prealloc           \
			-smp 5
		;;
	*)
        echo "error: unsupported target"
		;;
esac
