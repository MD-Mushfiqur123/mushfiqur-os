#!/bin/bash
# Build bootable ISO for Mushfiqur OS using El Torito (no-emulation boot)

echo "========================================="
echo "  Creating Mushfiqur OS Bootable ISO"
echo "========================================="
echo ""

# Build the OS first
echo "Building OS..."
make all

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Create ISO directory structure
echo "Setting up ISO directory..."
mkdir -p iso/boot

# Copy the bootable disk image
echo "Copying bootable image..."
cp build/mushfiqur-os.img iso/boot/mushfiqur.img

# Create ISO with El Torito boot (no emulation mode)
echo "Creating bootable ISO..."

if command -v genisoimage &> /dev/null; then
    genisoimage -R -J -c boot/boot.cat \
        -b boot/mushfiqur.img \
        -no-emul-boot \
        -boot-load-size 4 \
        -o mushfiqur-os.iso \
        iso/
    echo "ISO created with genisoimage"
elif command -v mkisofs &> /dev/null; then
    mkisofs -R -J -c boot/boot.cat \
        -b boot/mushfiqur.img \
        -no-emul-boot \
        -boot-load-size 4 \
        -o mushfiqur-os.iso \
        iso/
    echo "ISO created with mkisofs"
elif command -v xorriso &> /dev/null; then
    xorriso -as mkisofs -R -J -c boot/boot.cat \
        -b boot/mushfiqur.img \
        -no-emul-boot \
        -boot-load-size 4 \
        -o mushfiqur-os.iso \
        iso/
    echo "ISO created with xorriso"
else
    echo "Error: No ISO creation tool found!"
    echo "Install with: sudo apt-get install genisoimage"
    exit 1
fi

if [ -f mushfiqur-os.iso ]; then
    echo ""
    echo "========================================="
    echo "  ISO Created Successfully!"
    echo "========================================="
    echo ""
    echo "ISO file: mushfiqur-os.iso"
    echo "Size: $(du -h mushfiqur-os.iso | cut -f1)"
    echo ""
    echo "To run in QEMU:"
    echo "  qemu-system-i386 -cdrom mushfiqur-os.iso"
    echo ""
    echo "To run in VirtualBox:"
    echo "  1. Create new VM (Other, 32-bit)"
    echo "  2. Attach mushfiqur-os.iso as CD/DVD"
    echo "  3. Boot from CD"
    echo ""
else
    echo "Error: ISO creation failed!"
    exit 1
fi
