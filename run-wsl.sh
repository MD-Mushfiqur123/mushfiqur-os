#!/bin/bash
# Run QEMU with proper display settings for WSL

echo "========================================="
echo "  Mushfiqur OS - WSL Boot Fix"
echo "========================================="
echo ""

# Check if we're in WSL
if grep -qi microsoft /proc/version; then
    echo "Detected WSL environment"
    echo ""
    
    # Try to use WSLg display (WSL2 with GUI support)
    if [ -z "$DISPLAY" ]; then
        export DISPLAY=:0
        echo "Set DISPLAY=:0 for WSLg"
    fi
    
    echo "Display: $DISPLAY"
    echo ""
fi

echo "Starting QEMU..."
echo "If you don't see a window, you may need to install an X server"
echo ""

# Run QEMU with SDL or GTK display
qemu-system-i386 \
    -drive format=raw,file=build/mushfiqur-os.img \
    -display sdl \
    -m 512M

# If that fails, try with GTK
if [ $? -ne 0 ]; then
    echo ""
    echo "SDL failed, trying GTK..."
    qemu-system-i386 \
        -drive format=raw,file=build/mushfiqur-os.img \
        -display gtk \
        -m 512M
fi

# If that also fails, try VNC
if [ $? -ne 0 ]; then
    echo ""
    echo "GTK failed, starting VNC server..."
    echo "Connect to localhost:5900 with a VNC client"
    qemu-system-i386 \
        -drive format=raw,file=build/mushfiqur-os.img \
        -vnc :0 \
        -m 512M
fi
