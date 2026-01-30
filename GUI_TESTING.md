# Mushfiqur OS - GUI Testing Guide

## Quick Start

### Build the OS
```bash
cd /mnt/c/Users/20mah/.gemini/antigravity/scratch/mushfiqur-os
make clean
make all
```

### Run in QEMU
```bash
make run
```

Or manually:
```bash
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img
```

## Boot Options

When the OS boots, you'll see:
```
Select mode:
  [T] Text Mode (Shell)
  [G] Graphics Mode (GUI)

Press T or G:
```

### Text Mode (T)
- Traditional command-line shell
- Commands: help, about, version, clear, echo, shutdown

### Graphics Mode (G)
- VGA 320x200 graphics
- Mouse cursor
- Desktop with icons
- Draggable windows
- GUI applications

## GUI Features

### Desktop
- Blue gradient background
- Taskbar at bottom
- Start button
- Clock display
- Desktop icons

### Applications
1. **Terminal** - Click to open GUI terminal
2. **About** - Shows OS information
3. **Calculator** - Calculator interface

### Window Controls
- **Drag**: Click title bar and drag
- **Close**: Click red X button
- **Focus**: Click anywhere on window

## Troubleshooting

### If OS doesn't boot:
1. Make sure QEMU is installed
2. Rebuild: `make clean && make all`
3. Check build output for errors

### If GUI doesn't appear:
- Make sure you pressed 'G' not 'T'
- VGA Mode 13h should activate automatically
- Screen should show blue gradient background

### If mouse doesn't work:
- QEMU should enable mouse automatically
- Click in QEMU window to capture mouse
- Press Ctrl+Alt+G to release mouse

## Technical Details

- **Graphics**: VGA Mode 13h (320x200, 256 colors)
- **Input**: PS/2 Mouse + Keyboard
- **Window System**: Custom window manager
- **Applications**: 3 built-in apps

## Fixed Issues

✅ VGA mode switching now uses direct hardware programming  
✅ Works in protected mode (no BIOS interrupts)  
✅ All GUI components compile successfully  
✅ Mouse driver integrated  
✅ Window manager functional  

Enjoy your GUI-enabled OS!
