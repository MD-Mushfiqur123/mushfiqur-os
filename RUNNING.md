# Mushfiqur OS - Running Guide

## Recommended: Run Disk Image Directly

The **best way** to run Mushfiqur OS is using the raw disk image:

```bash
# Build
make all

# Run in QEMU
make run
```

Or manually:
```bash
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img
```

This works perfectly and boots every time!

## Why Not ISO?

The ISO boot has issues because:
- Our bootloader is designed for hard disk boot
- CD-ROM boot uses different BIOS calls
- The disk reading code expects a hard disk, not CD-ROM

## Alternative: Use VirtualBox with Disk Image

You can also use the disk image in VirtualBox:

1. Convert to VDI format:
```bash
VBoxManage convertfromraw build/mushfiqur-os.img mushfiqur-os.vdi --format VDI
```

2. Create VM in VirtualBox
3. Attach `mushfiqur-os.vdi` as hard disk
4. Boot!

## Or: Use QEMU (Recommended)

QEMU works perfectly with the raw disk image:

```bash
# Standard boot
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img

# With more memory
qemu-system-i386 -m 512M -drive format=raw,file=build/mushfiqur-os.img

# With mouse support
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img -usb -device usb-mouse
```

## Boot Process

1. **Bootloader** loads from sector 0
2. **Kernel** loads from sectors 1-20
3. **Mode selection** (T or G)
4. **OS starts**!

## What Works

✅ Disk image boot (QEMU)  
✅ Disk image boot (VirtualBox with VDI)  
✅ Text mode shell  
✅ Graphics mode GUI  
✅ Keyboard input  
✅ Mouse input  

## Quick Commands

```bash
# Build
make clean && make all

# Run
make run

# Clean
make clean
```

---

**Use the disk image - it works perfectly!** 🚀
