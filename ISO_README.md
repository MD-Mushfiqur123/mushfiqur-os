# 💿 Mushfiqur OS - ISO Image

## What is this?

A bootable ISO image of Mushfiqur OS that can be:
- Burned to CD/DVD
- Used in virtual machines (VirtualBox, VMware)
- Booted from USB (with proper tools)
- Run in QEMU

## Quick Start

### Create ISO
```bash
make iso
```

Or manually:
```bash
./make-iso.sh
```

### Run ISO in QEMU
```bash
make run-iso
```

Or manually:
```bash
qemu-system-i386 -cdrom mushfiqur-os.iso
```

## ISO Details

- **File**: `mushfiqur-os.iso`
- **Size**: ~12 MB
- **Bootloader**: GRUB
- **Format**: ISO 9660 with El Torito boot

## Boot Menu

When you boot from the ISO, you'll see:

```
Mushfiqur OS v1.0.0 - Genesis Edition
Mushfiqur OS (Safe Mode)
```

Select an option and press Enter.

## Using in VirtualBox

1. Open VirtualBox
2. Create new VM:
   - Type: Other
   - Version: Other/Unknown (32-bit)
   - Memory: 512 MB (minimum)
   - No hard disk needed
3. Settings → Storage → Add Optical Drive
4. Select `mushfiqur-os.iso`
5. Start the VM

## Using in VMware

1. Create new VM
2. Select "I will install the operating system later"
3. Guest OS: Other → Other (32-bit)
4. Edit VM settings
5. CD/DVD → Use ISO image → Select `mushfiqur-os.iso`
6. Power on VM

## Burning to CD/DVD

### Windows
- Use ImgBurn, CDBurnerXP, or Windows built-in burner
- Right-click ISO → Burn disc image

### Linux
```bash
wodim -v mushfiqur-os.iso
```

### macOS
```bash
hdiutil burn mushfiqur-os.iso
```

## Creating Bootable USB

### Linux (using dd)
```bash
sudo dd if=mushfiqur-os.iso of=/dev/sdX bs=4M status=progress
sudo sync
```
**Warning**: Replace `/dev/sdX` with your USB device!

### Windows
- Use Rufus or balenaEtcher
- Select ISO
- Select USB drive
- Write

## Technical Details

### GRUB Configuration
Located in `iso/boot/grub/grub.cfg`:
- 5-second timeout
- Default boot entry
- Multiboot specification

### ISO Structure
```
iso/
├── boot/
│   ├── grub/
│   │   ├── grub.cfg
│   │   └── i386-pc/ (GRUB modules)
│   └── mushfiqur-os.bin
```

### Build Process
1. Build OS image (`make all`)
2. Create ISO directory structure
3. Copy kernel to `iso/boot/`
4. Generate GRUB configuration
5. Create ISO with `grub-mkrescue` or `xorriso`

## Troubleshooting

### ISO won't boot
- Ensure BIOS/UEFI is set to boot from CD/DVD
- Try "Safe Mode" option in GRUB menu
- Check VM settings for boot order

### GRUB error
- Rebuild ISO: `make clean && make iso`
- Ensure GRUB tools are installed

### File not found
- Make sure you built the OS first: `make all`
- Check that `build/mushfiqur-os.img` exists

## Requirements

### Build Requirements
- `grub-mkrescue` or `xorriso`
- `grub-pc-bin` (for GRUB modules)

### Install on Ubuntu/Debian
```bash
sudo apt-get install grub-pc-bin xorriso
```

### Install on Fedora/RHEL
```bash
sudo dnf install grub2-tools-extra xorriso
```

## Makefile Targets

- `make iso` - Create ISO image
- `make run-iso` - Create and run ISO in QEMU
- `make clean` - Remove build files and ISO

## What's Included

✅ Bootloader (GRUB)  
✅ Kernel with GUI support  
✅ VGA graphics driver  
✅ Mouse driver  
✅ Window manager  
✅ Desktop environment  
✅ 3 GUI applications  
✅ Text-mode shell  

## File Size Breakdown

- GRUB bootloader: ~8 MB
- Mushfiqur OS kernel: ~50 KB
- Total ISO: ~12 MB

---

**Enjoy your bootable Mushfiqur OS ISO!** 💿🚀
