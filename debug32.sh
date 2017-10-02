#!/bin/sh

KERNELNAME=os351

QEMU="qemu-system-i386 -s -S"
QEMU_FLAGS="-smp 2"

$QEMU $QEMU_FLAGS -cdrom $KERNELNAME.iso
