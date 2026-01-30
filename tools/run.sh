#!/bin/bash
# Run script for Mushfiqur OS using QEMU

echo "Starting Mushfiqur OS in QEMU..."
echo ""
echo "Press Ctrl+Alt+G to release mouse/keyboard from QEMU"
echo "Press Ctrl+C in this terminal to stop QEMU"
echo ""

# Check if OS image exists
if [ ! -f "build/mushfiqur-os.img" ]; then
    echo "Error: OS image not found!"
    echo "Please build the OS first using: make all"
    exit 1
fi

# Check if QEMU is installed
command -v qemu-system-i386 >/dev/null 2>&1 || {
    echo "Error: QEMU not found. Please install QEMU."
    exit 1
}

# Run QEMU
qemu-system-i386 \
    -drive format=raw,file=build/mushfiqur-os.img \
    -m 32M \
    -monitor stdio

echo ""
echo "Mushfiqur OS stopped."
