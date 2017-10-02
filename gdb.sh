#!/bin/sh

if [ $# -lt 1 ]; then
	gdb os351.elf -ex "target remote :1234"
else
	gdb os351.elf -ex "target remote $1:1234"
fi

