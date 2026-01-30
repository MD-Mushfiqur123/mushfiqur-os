# Mushfiqur OS Architecture

## Overview

Mushfiqur OS is a 32-bit protected mode operating system built from scratch for the x86 architecture. It features a monolithic kernel design with integrated device drivers and a custom bootloader.

## System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    User Space                           │
│  ┌──────────────────────────────────────────────────┐  │
│  │              Shell (CLI)                          │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↕
┌─────────────────────────────────────────────────────────┐
│                   Kernel Space                          │
│  ┌──────────────────────────────────────────────────┐  │
│  │         Kernel Core (kernel.c)                   │  │
│  │  - Initialization                                 │  │
│  │  - Port I/O                                       │  │
│  │  - System Management                              │  │
│  └──────────────────────────────────────────────────┘  │
│                          ↕                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │         Memory Manager (memory.c)                │  │
│  │  - Heap Allocator (kmalloc/kfree)                │  │
│  │  - Memory Operations (memcpy, memset)            │  │
│  └──────────────────────────────────────────────────┘  │
│                          ↕                              │
│  ┌──────────────────────────────────────────────────┐  │
│  │            Device Drivers                         │  │
│  │  ┌────────────────┐  ┌────────────────────────┐  │  │
│  │  │ Screen Driver  │  │  Keyboard Driver       │  │  │
│  │  │  (screen.c)    │  │   (keyboard.c)         │  │  │
│  │  └────────────────┘  └────────────────────────┘  │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↕
┌─────────────────────────────────────────────────────────┐
│                    Hardware Layer                       │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │   CPU    │  │  Memory  │  │   I/O    │             │
│  └──────────┘  └──────────┘  └──────────┘             │
└─────────────────────────────────────────────────────────┘
```

## Boot Sequence

### Stage 1: BIOS Boot (Real Mode)

1. **Power On**: CPU starts in 16-bit real mode
2. **BIOS POST**: Hardware initialization and testing
3. **Boot Device Selection**: BIOS identifies bootable device
4. **Boot Sector Load**: BIOS loads first 512 bytes to `0x7C00`
5. **Boot Signature Check**: BIOS verifies `0xAA55` signature
6. **Transfer Control**: Jump to `0x7C00`

### Stage 2: Bootloader Execution

```
boot.asm (0x7C00)
    ↓
1. Set up stack (BP=0x9000, SP=0x9000)
    ↓
2. Print boot message (Real mode)
    ↓
3. Load kernel from disk
   - Read 31 sectors starting from sector 2
   - Load to memory address 0x1000
   - Use BIOS INT 0x13 (disk services)
    ↓
4. Set up Global Descriptor Table (GDT)
   - Null descriptor
   - Code segment (0x00 - 0xFFFFFFFF)
   - Data segment (0x00 - 0xFFFFFFFF)
    ↓
5. Switch to Protected Mode
   - Disable interrupts (CLI)
   - Load GDT (LGDT)
   - Set PE bit in CR0
   - Far jump to flush pipeline
    ↓
6. Initialize 32-bit segments
   - Update DS, SS, ES, FS, GS
   - Set up 32-bit stack (ESP=0x90000)
    ↓
7. Jump to kernel at 0x1000
```

### Stage 3: Kernel Initialization

```
kernel_entry.asm
    ↓
1. Set up kernel stack (ESP = kernel_stack_top)
    ↓
2. Call kernel_main()
    ↓
kernel_main() in kernel.c
    ↓
3. Clear screen
    ↓
4. Print boot banner
    ↓
5. Initialize memory manager
    ↓
6. Initialize keyboard driver
    ↓
7. Start shell
    ↓
8. Enter main loop (halt)
```

## Memory Map

### Physical Memory Layout

```
0x00000000 - 0x000003FF : Real Mode IVT (Interrupt Vector Table)
0x00000400 - 0x000004FF : BIOS Data Area
0x00000500 - 0x00007BFF : Free conventional memory
0x00007C00 - 0x00007DFF : Bootloader (512 bytes)
0x00007E00 - 0x0007FFFF : Free conventional memory
0x00080000 - 0x0009FFFF : Extended BIOS Data Area
0x000A0000 - 0x000BFFFF : Video memory
0x000B8000 - 0x000B8FA0 : VGA Text Mode Buffer (80x25x2 = 4000 bytes)
0x000C0000 - 0x000FFFFF : BIOS ROM
0x00100000 - 0x001FFFFF : Kernel heap (1 MB)
0x00001000 - 0x000FFFFF : Kernel code and data
```

### Kernel Memory Sections

```
Linker Script (linker.ld):

0x1000:
    .text    : Kernel code (executable)
    .rodata  : Read-only data (constants, strings)
    .data    : Initialized data (global variables)
    .bss     : Uninitialized data (zero-initialized)
```

## Component Details

### Bootloader Components

#### boot.asm
- Main bootloader entry point
- Orchestrates boot process
- Includes all sub-modules

#### print.asm
- Real mode string printing (INT 0x10)
- Hex value printing for debugging
- Uses BIOS teletype mode

#### disk.asm
- BIOS disk reading (INT 0x13)
- Error handling
- Multi-sector loading

#### gdt.asm
- Global Descriptor Table definition
- Code segment descriptor (executable, readable)
- Data segment descriptor (writable)
- Flat memory model (0x00000000 - 0xFFFFFFFF)

#### switch_pm.asm
- Protected mode transition
- Segment register initialization
- 32-bit print function for protected mode

### Kernel Components

#### kernel_entry.asm
- Assembly entry point
- Stack setup
- Calls C kernel main

#### kernel.c
- Main kernel initialization
- Port I/O functions (inb, outb, inw, outw)
- Kernel panic handler
- System halt function

#### memory.c
- Simple bump allocator
- Heap: 1 MB starting at 0x100000
- kmalloc: allocate memory (4-byte aligned)
- kfree: placeholder (no actual freeing)
- Memory utility functions

### Device Drivers

#### screen.c (VGA Text Mode Driver)

**Features:**
- 80x25 character display
- 16 color support (foreground/background)
- Hardware cursor control
- Automatic scrolling
- Character and string printing

**VGA Memory:**
- Base address: 0xB8000
- Format: [Character][Attribute] (2 bytes per cell)
- Attribute: [Background (4 bits)][Foreground (4 bits)]

**Cursor Control:**
- Port 0x3D4: VGA index register
- Port 0x3D5: VGA data register
- Registers 14-15: cursor position

#### keyboard.c (PS/2 Keyboard Driver)

**Features:**
- Scancode to ASCII conversion
- Shift key support
- Special key handling (Enter, Backspace)
- Callback mechanism for key events

**Hardware:**
- Data port: 0x60 (read scancodes)
- Status port: 0x64 (check data availability)

**Scancode Handling:**
- Make codes: key press (bit 7 = 0)
- Break codes: key release (bit 7 = 1)
- Two scancode tables (normal/shift)

### Shell

#### shell.c

**Features:**
- Command-line interface
- Input buffering (256 characters)
- Command parsing
- Built-in commands
- Colored prompt

**Commands:**
- `help`: Display command list
- `about`: OS information
- `version`: Version details
- `clear`: Clear screen
- `echo <text>`: Echo text
- `shutdown`: Halt system

**Input Handling:**
- Character-by-character input
- Backspace support
- Enter to execute
- Visual feedback

## Data Flow Examples

### Keyboard Input Flow

```
1. User presses key
    ↓
2. Keyboard controller sends scancode to port 0x60
    ↓
3. keyboard_get_char() polls port 0x60
    ↓
4. Scancode converted to ASCII using lookup table
    ↓
5. Shift state checked for uppercase/symbols
    ↓
6. ASCII character returned
    ↓
7. shell_key_handler() processes character
    ↓
8. Character added to input buffer or command executed
    ↓
9. Screen updated via screen driver
```

### Screen Output Flow

```
1. kprint("Hello") called
    ↓
2. kprint_color() called with WHITE color
    ↓
3. For each character:
    ↓
4. screen_write_char() called
    ↓
5. Calculate VGA memory offset (row * 80 + col) * 2
    ↓
6. Write character to VGA memory[offset]
    ↓
7. Write color attribute to VGA memory[offset + 1]
    ↓
8. Update cursor position
    ↓
9. Check if scrolling needed
    ↓
10. Update hardware cursor via ports 0x3D4/0x3D5
```

## Design Decisions

### Monolithic Kernel
- **Pros**: Simple design, fast communication between components
- **Cons**: Less modular, harder to debug
- **Rationale**: Appropriate for educational OS, easier to implement

### Bump Allocator
- **Pros**: Simple, fast allocation
- **Cons**: No memory reclamation, fragmentation
- **Rationale**: Sufficient for current feature set, easy to understand

### Polling-Based Keyboard
- **Pros**: Simple implementation, no interrupt handling needed
- **Cons**: CPU intensive, no multitasking support
- **Rationale**: Adequate for single-tasking shell environment

### VGA Text Mode
- **Pros**: Simple, well-documented, no graphics library needed
- **Cons**: Limited to 80x25 text, no graphics
- **Rationale**: Perfect for command-line interface

## Future Enhancements

### Interrupt Handling
- Implement IDT (Interrupt Descriptor Table)
- Set up PIC (Programmable Interrupt Controller)
- Keyboard IRQ handler (IRQ1)
- Timer IRQ handler (IRQ0)

### Process Management
- Process Control Block (PCB) structure
- Context switching
- Round-robin scheduler
- Process states (ready, running, blocked)

### File System
- FAT12 or custom file system
- Disk I/O operations
- File operations (open, read, write, close)
- Directory management

### Graphical Interface
- VGA graphics mode (320x200, 256 colors)
- Pixel plotting
- Basic shapes and text rendering
- Window manager
- Mouse support

## References

- Intel 80386 Programmer's Reference Manual
- OSDev Wiki: https://wiki.osdev.org
- VGA Hardware Documentation
- PS/2 Keyboard Protocol Specification
