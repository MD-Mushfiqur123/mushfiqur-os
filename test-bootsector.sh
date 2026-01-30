#!/bin/bash
# Direct boot test

echo "Testing boot sector directly..."
echo ""
echo "Boot sector info:"
file build/boot.bin
ls -lh build/boot.bin
echo ""
echo "Boot signature check:"
od -An -tx1 -j 510 -N 2 build/boot.bin
echo ""
echo "Starting QEMU with boot sector only..."
echo "You should see 'M' if bootloader runs"
echo ""

qemu-system-i386 -drive format=raw,file=build/boot.bin
