# Mushfiqur OS - Quick Start Guide

## What is Mushfiqur OS?

Mushfiqur OS is a complete, custom-built operating system created from scratch. It features:

- ✅ **Custom Bootloader** - Boots from bare metal
- ✅ **32-bit Protected Mode Kernel** - Full kernel implementation
- ✅ **VGA Display Driver** - Colorful text-mode interface
- ✅ **Keyboard Driver** - Full keyboard input support
- ✅ **Memory Management** - Dynamic memory allocation
- ✅ **Interactive Shell** - Command-line interface with built-in commands

## Quick Start

### Prerequisites

You need these tools installed:
- **NASM** - Assembler for bootloader
- **GCC** - C compiler (with 32-bit support)
- **Make** - Build automation
- **QEMU** - Emulator for testing

### Windows Installation (using Chocolatey)

```powershell
choco install nasm mingw make qemu
```

### Build the OS

```bash
cd mushfiqur-os
make all
```

This creates `build/mushfiqur-os.img` - a bootable OS image!

### Run the OS

```bash
make run
```

Or manually:
```bash
qemu-system-i386 -drive format=raw,file=build/mushfiqur-os.img
```

## Using the OS

Once booted, you'll see the Mushfiqur Shell. Try these commands:

```bash
help        # Show all commands
about       # OS information
version     # Version details
echo Hello  # Echo text
clear       # Clear screen
shutdown    # Halt system
```

## Project Structure

```
mushfiqur-os/
├── boot/              # Bootloader (Assembly)
│   ├── boot.asm       # Main bootloader
│   ├── print.asm      # Print functions
│   ├── disk.asm       # Disk loading
│   ├── gdt.asm        # Global Descriptor Table
│   └── switch_pm.asm  # Protected mode switch
│
├── kernel/            # Kernel (C + Assembly)
│   ├── kernel_entry.asm  # Entry point
│   ├── kernel.c          # Main kernel
│   └── memory.c          # Memory manager
│
├── drivers/           # Device Drivers (C)
│   ├── screen.c       # VGA display
│   └── keyboard.c     # PS/2 keyboard
│
├── shell/             # User Interface (C)
│   └── shell.c        # Command shell
│
├── include/           # Header files
│   ├── kernel.h
│   ├── types.h
│   ├── memory.h
│   └── drivers/
│       ├── screen.h
│       └── keyboard.h
│
├── build/             # Build output
├── tools/             # Build scripts
├── docs/              # Documentation
├── Makefile           # Build system
└── linker.ld          # Linker script
```

## How It Works

### Boot Process

1. **BIOS** loads bootloader (512 bytes) at memory address 0x7C00
2. **Bootloader** loads kernel from disk into memory
3. **GDT** (Global Descriptor Table) is configured
4. **Protected Mode** is enabled (16-bit → 32-bit)
5. **Kernel** initializes memory, drivers, and shell
6. **Shell** waits for user input

### Memory Layout

```
0x00007C00  : Bootloader (512 bytes)
0x00001000  : Kernel code and data
0x000B8000  : VGA text mode buffer (80x25 chars)
0x00100000  : Heap (1 MB for dynamic allocation)
```

## Technical Details

### Bootloader
- Written in x86 Assembly (NASM)
- Loads kernel using BIOS interrupts
- Sets up GDT for protected mode
- Switches CPU from 16-bit to 32-bit mode

### Kernel
- Written in C with Assembly entry point
- Monolithic design (all in kernel space)
- Port I/O for hardware communication
- Simple bump allocator for memory

### Drivers
- **Screen**: VGA text mode (80x25), 16 colors, hardware cursor
- **Keyboard**: PS/2 scancode conversion, shift key support

### Shell
- Command parsing and execution
- Input buffering
- Built-in commands
- Colored output

## Development

### Building from Source

```bash
# Clean previous builds
make clean

# Build everything
make all

# Run in QEMU
make run
```

### Modifying the OS

1. Edit source files in `kernel/`, `drivers/`, or `shell/`
2. Run `make clean && make all` to rebuild
3. Test with `make run`

### Adding New Commands

Edit `shell/shell.c`:

```c
void cmd_mycommand() {
    kprint("My custom command!\n");
}

// In process_command():
else if (strcmp(cmd, "mycommand") == 0) {
    cmd_mycommand();
}
```

## Troubleshooting

### Build Errors

**"nasm: command not found"**
→ Install NASM assembler

**"gcc: unrecognized option '-m32'"**
→ Install 32-bit GCC libraries: `apt-get install gcc-multilib`

### Runtime Issues

**Black screen**
→ Check QEMU command, ensure image file exists

**Keyboard not working**
→ Click inside QEMU window to give it focus

**Bootloader error**
→ Check boot sector size (must be exactly 512 bytes)

## Next Steps

Want to extend the OS? Here are some ideas:

- [ ] Add more shell commands (ls, cat, mkdir)
- [ ] Implement interrupt handling (IDT)
- [ ] Add a timer driver
- [ ] Create a simple file system
- [ ] Build a graphical interface
- [ ] Add process management
- [ ] Implement system calls

## Resources

- **README.md** - Full documentation
- **docs/ARCHITECTURE.md** - Detailed architecture
- **OSDev Wiki** - https://wiki.osdev.org
- **Intel Manuals** - x86 architecture reference

## Credits

**Mushfiqur OS v1.0.0 (Genesis)**  
Created by: Mushfiqur Rahman  
Year: 2026

Built with passion for operating systems! 🚀

---

**Note**: This is an educational operating system. It demonstrates fundamental OS concepts like bootloading, memory management, device drivers, and user interaction. Perfect for learning how operating systems work at the lowest level!
