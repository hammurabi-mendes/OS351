#!/bin/sh

KERNELNAME=os351

QEMU="qemu-system-x86_64 -s -S"
QEMU_FLAGS="-smp 2"

$QEMU $QEMU_FLAGS -cdrom $KERNELNAME.iso
