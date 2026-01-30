#!/bin/bash
# Simple boot test

echo "========================================="
echo "  Mushfiqur OS - Boot Test"
echo "========================================="
echo ""
echo "Building OS..."
make clean > /dev/null 2>&1
make all > /dev/null 2>&1

if [ ! -f build/mushfiqur-os.img ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo "Build successful!"
echo "Image size: $(du -h build/mushfiqur-os.img | cut -f1)"
echo ""
echo "Starting QEMU..."
echo "You should see boot messages in the QEMU window."
echo ""
echo "Expected boot sequence:"
echo "  1. 'Mushfiqur OS - Booting in 16-bit Real Mode...'"
echo "  2. 'Loading kernel into memory...'"
echo "  3. 'Kernel loaded successfully!'"
echo "  4. 'Successfully switched to 32-bit Protected Mode'"
echo "  5. Mode selection prompt"
echo ""
echo "Press Ctrl+Alt+G to release mouse from QEMU"
echo "Press Ctrl+C in this terminal to stop QEMU"
echo ""

# Run QEMU with simple options
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img
