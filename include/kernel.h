#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

// Kernel version
#define OS_NAME "Mushfiqur OS"
#define OS_VERSION "1.0.0"
#define OS_CODENAME "Genesis"

// Port I/O functions
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t value);
uint16_t inw(uint16_t port);

// Utility functions
void kernel_panic(const char* message);
void halt();

#endif
