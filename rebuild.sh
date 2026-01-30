#!/bin/bash
# Rebuild script for Mushfiqur OS

echo "========================================="
echo "  Rebuilding Mushfiqur OS"
echo "========================================="
echo ""

# Clean previous build
echo "Cleaning previous build..."
make clean

echo ""
echo "Building OS..."
make all

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "  Build Complete!"
    echo "========================================="
    echo ""
    echo "OS image: build/mushfiqur-os.img"
    echo ""
    echo "To run: make run"
    echo ""
else
    echo ""
    echo "Build failed!"
    exit 1
fi
