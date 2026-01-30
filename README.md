# 🚀 Mushfiqur OS

A complete operating system built from scratch with custom bootloader, kernel, and graphical user interface.

![OS Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-x86-orange)

## ✨ Features

- **Custom Bootloader** - 512-byte boot sector written in Assembly
- **32-bit Protected Mode Kernel** - Written in C
- **VGA Graphics Driver** - Mode 13h (320x200, 256 colors)
- **Window Manager** - Draggable windows with title bars
- **Desktop Environment** - Taskbar, icons, and system tray
- **PS/2 Mouse Support** - Full mouse input handling
- **GUI Applications** - Terminal, About dialog, Calculator
- **Text Mode Shell** - Command-line interface
- **Memory Management** - Custom heap allocator

## 🖼️ Screenshots

Boot into either text mode or graphics mode:
- **Text Mode**: Classic command-line shell
- **Graphics Mode**: Full GUI with windows and mouse

## 🛠️ Building

### Requirements
- NASM (assembler)
- GCC (32-bit cross-compiler)
- LD (linker)
- Make
- QEMU (for testing)

### Ubuntu/Debian
```bash
sudo apt-get install nasm gcc-multilib qemu-system-x86
```

### Build Commands
```bash
# Build the OS
make clean && make all

# Run in QEMU (requires X server in WSL)
make run

# Create ISO image
make iso
```

## 🚀 Running

### Option 1: QEMU (Recommended)
```bash
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img
```

### Option 2: VirtualBox
1. Convert to VDI: `VBoxManage convertfromraw build/mushfiqur-os.img mushfiqur-os.vdi --format VDI`
2. Create VM and attach VDI
3. Boot!

### Option 3: Real Hardware
Write the image to a USB drive (⚠️ **WARNING: This will erase the drive!**):
```bash
sudo dd if=build/mushfiqur-os.img of=/dev/sdX bs=4M
```

## 🎮 Usage

### Boot Menu
When the OS boots, you'll see:
```
Select mode:
  [T] Text Mode (Shell)
  [G] Graphics Mode (GUI)
```

### Text Mode Commands
- `help` - Show available commands
- `about` - OS information
- `version` - Show version
- `clear` - Clear screen
- `echo <text>` - Print text
- `shutdown` - Halt system

### Graphics Mode
- Click desktop icons to launch applications
- Drag windows by title bar
- Click red X to close windows
- Mouse cursor follows your movements

## 📁 Project Structure

```
mushfiqur-os/
├── boot/           # Bootloader code
│   ├── boot.asm    # Main bootloader
│   ├── disk.asm    # Disk I/O
│   ├── gdt.asm     # Global Descriptor Table
│   └── print.asm   # Print functions
├── kernel/         # Kernel code
│   ├── kernel.c    # Main kernel
│   └── memory.c    # Memory management
├── drivers/        # Device drivers
│   ├── screen.c    # Text mode VGA
│   ├── keyboard.c  # PS/2 keyboard
│   ├── vga.c       # Graphics VGA
│   └── mouse.c     # PS/2 mouse
├── gui/            # GUI system
│   ├── window.c    # Window manager
│   ├── desktop.c   # Desktop environment
│   └── gui.c       # GUI controller
├── apps/           # Applications
│   ├── terminal.c  # GUI terminal
│   ├── about.c     # About dialog
│   └── calculator.c # Calculator
├── shell/          # Text shell
│   └── shell.c     # Command interpreter
└── include/        # Header files
```

## 🔧 Technical Details

### Architecture
- **CPU**: x86 (32-bit)
- **Boot**: Legacy BIOS
- **Memory**: 512MB recommended
- **Graphics**: VGA Mode 13h

### Boot Process
1. BIOS loads 512-byte boot sector at `0x7C00`
2. Bootloader loads kernel from disk to `0x1000`
3. Switch to 32-bit protected mode
4. Initialize GDT and jump to kernel
5. Kernel initializes drivers and subsystems
6. User selects text or graphics mode
7. OS starts!

### Memory Map
```
0x00000000 - 0x000003FF : Interrupt Vector Table
0x00000400 - 0x000004FF : BIOS Data Area
0x00007C00 - 0x00007DFF : Bootloader
0x00001000 - 0x0000FFFF : Kernel
0x000A0000 - 0x000BFFFF : VGA Memory
0x00100000+             : Extended Memory
```

## 🐛 Known Issues

### WSL Display Issue
QEMU requires X server in WSL. Solutions:
1. **VNC**: `qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img -vnc :0`
2. **Install VcXsrv** and set `DISPLAY` variable
3. **Use WSL2 with WSLg** (Windows 11)

### ISO Boot
The ISO image has boot issues with GRUB multiboot. Use the raw disk image instead.

## 📝 License

MIT License - See LICENSE file

## 👨‍💻 Author

**Mushfiqur Rahman**

## 🙏 Acknowledgments

- OSDev Wiki for documentation
- QEMU team for the emulator
- The open source community

## 🎯 Future Enhancements

- [ ] File system (FAT12/FAT16)
- [ ] Process scheduler
- [ ] Networking stack
- [ ] Sound driver
- [ ] Higher resolution graphics modes
- [ ] More GUI applications
- [ ] USB support

---

**Built with ❤️ and Assembly**
