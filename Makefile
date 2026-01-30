# Mushfiqur OS Makefile

# Compiler and tools
ASM = nasm
CC = gcc
LD = ld

# Flags
ASM_FLAGS = -f elf32
CC_FLAGS = -m32 -ffreestanding -nostdlib -fno-pie -fno-stack-protector -Wall -Wextra -c
LD_FLAGS = -m elf_i386 -T linker.ld

# Directories
BUILD_DIR = build
BOOT_DIR = boot
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
SHELL_DIR = shell
INCLUDE_DIR = include

# Source files
BOOT_SRC = $(BOOT_DIR)/boot.asm
KERNEL_ENTRY_SRC = $(KERNEL_DIR)/kernel_entry.asm
KERNEL_SRC = $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/memory.c
DRIVERS_SRC = $(DRIVERS_DIR)/screen.c $(DRIVERS_DIR)/keyboard.c $(DRIVERS_DIR)/vga.c $(DRIVERS_DIR)/mouse.c
SHELL_SRC = $(SHELL_DIR)/shell.c
GUI_SRC = gui/window.c gui/desktop.c gui/gui.c
APPS_SRC = apps/terminal.c apps/about.c apps/calculator.c

# Object files
BOOT_BIN = $(BUILD_DIR)/boot.bin
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ = $(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))
DRIVERS_OBJ = $(patsubst $(DRIVERS_DIR)/%.c,$(BUILD_DIR)/%.o,$(DRIVERS_SRC))
SHELL_OBJ = $(patsubst $(SHELL_DIR)/%.c,$(BUILD_DIR)/%.o,$(SHELL_SRC))
GUI_OBJ = $(patsubst gui/%.c,$(BUILD_DIR)/%.o,$(GUI_SRC))
APPS_OBJ = $(patsubst apps/%.c,$(BUILD_DIR)/%.o,$(APPS_SRC))

ALL_OBJ = $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ) $(DRIVERS_OBJ) $(SHELL_OBJ) $(GUI_OBJ) $(APPS_OBJ)

# Output
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/mushfiqur-os.img

.PHONY: all clean run

all: $(OS_IMAGE)

# Create OS image
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "Creating OS image..."
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	@# Pad to ensure proper sector alignment (optional but recommended)
	@truncate -s 20K $(OS_IMAGE) 2>/dev/null || true
	@echo "OS image created: $(OS_IMAGE)"

# Build bootloader
$(BOOT_BIN): $(BOOT_SRC)
	@echo "Building bootloader..."
	$(ASM) -f bin $(BOOT_SRC) -o $(BOOT_BIN)

# Build kernel binary
$(KERNEL_BIN): $(ALL_OBJ)
	@echo "Linking kernel..."
	$(LD) $(LD_FLAGS) -o $(KERNEL_BIN) $(ALL_OBJ)

# Build kernel entry
$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_SRC)
	@echo "Assembling kernel entry..."
	$(ASM) $(ASM_FLAGS) $(KERNEL_ENTRY_SRC) -o $(KERNEL_ENTRY_OBJ)

# Build kernel C files
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) $< -o $@

# Build driver files
$(BUILD_DIR)/%.o: $(DRIVERS_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) $< -o $@

# Build shell files
$(BUILD_DIR)/%.o: $(SHELL_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) $< -o $@

# Build GUI files
$(BUILD_DIR)/%.o: gui/%.c
	@echo "Compiling $<..."
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) $< -o $@

# Build apps files
$(BUILD_DIR)/%.o: apps/%.c
	@echo "Compiling $<..."
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) $< -o $@

# Create bootable ISO image
iso: $(OS_IMAGE)
	@echo "Creating ISO image..."
	@./make-iso.sh

# Run in QEMU
run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

# Run ISO in QEMU
run-iso: iso
	qemu-system-i386 -cdrom mushfiqur-os.iso

# Clean build files
clean:
	rm -f $(BUILD_DIR)/*
	rm -rf iso/
	rm -f mushfiqur-os.iso

.PHONY: all clean run iso run-iso
