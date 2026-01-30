#!/bin/bash
# Test boot script

echo "Testing Mushfiqur OS boot..."
echo ""
echo "Starting QEMU..."
echo "Press Ctrl+C to exit"
echo ""

qemu-system-i386 \
    -drive format=raw,file=build/mushfiqur-os.img \
    -m 512M \
    -serial stdio \
    -display gtk
