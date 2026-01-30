#!/bin/bash
# Build script for Mushfiqur OS

echo "=================================="
echo "  Mushfiqur OS Build Script"
echo "=================================="
echo ""

# Check for required tools
echo "Checking for required tools..."

command -v nasm >/dev/null 2>&1 || { echo "Error: NASM not found. Please install NASM."; exit 1; }
command -v gcc >/dev/null 2>&1 || { echo "Error: GCC not found. Please install GCC."; exit 1; }
command -v ld >/dev/null 2>&1 || { echo "Error: LD not found. Please install binutils."; exit 1; }

echo "✓ All required tools found"
echo ""

# Create build directory
echo "Creating build directory..."
mkdir -p build
echo "✓ Build directory ready"
echo ""

# Build the OS
echo "Building Mushfiqur OS..."
make all

if [ $? -eq 0 ]; then
    echo ""
    echo "=================================="
    echo "  Build Successful!"
    echo "=================================="
    echo ""
    echo "OS image created: build/mushfiqur-os.img"
    echo ""
    echo "To run the OS, use:"
    echo "  make run"
    echo "  or"
    echo "  qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img"
    echo ""
else
    echo ""
    echo "=================================="
    echo "  Build Failed!"
    echo "=================================="
    echo ""
    exit 1
fi
