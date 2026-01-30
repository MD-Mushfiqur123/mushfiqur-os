#include <stdbool.h>
#include "../include/drivers/mouse.h"
#include "../include/drivers/vga.h"
#include "../include/kernel.h"

static MouseState mouse_state = {160, 100, false, false, false};

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout--) {
            if ((inb(0x64) & 1) == 1) return;
        }
    } else {
        while (timeout--) {
            if ((inb(0x64) & 2) == 0) return;
        }
    }
}

void mouse_write(uint8_t data) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, data);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_init() {
    // Enable auxiliary mouse device
    mouse_wait(1);
    outb(0x64, 0xA8);
    
    // Enable interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    uint8_t status = mouse_read() | 2;
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);
    
    // Use default settings
    mouse_write(0xF6);
    mouse_read();
    
    // Enable data reporting
    mouse_write(0xF4);
    mouse_read();
    
    mouse_state.x = VGA_WIDTH / 2;
    mouse_state.y = VGA_HEIGHT / 2;
}

void mouse_handler() {
    // Read mouse packet (simplified - would normally use interrupts)
    uint8_t status = inb(0x64);
    if (!(status & 0x20)) return;
    
    uint8_t packet[3];
    packet[0] = inb(0x60);
    packet[1] = inb(0x60);
    packet[2] = inb(0x60);
    
    // Update button states
    mouse_state.left_button = packet[0] & 0x01;
    mouse_state.right_button = packet[0] & 0x02;
    mouse_state.middle_button = packet[0] & 0x04;
    
    // Update position
    int dx = packet[1] - ((packet[0] << 4) & 0x100);
    int dy = packet[2] - ((packet[0] << 3) & 0x100);
    
    mouse_state.x += dx;
    mouse_state.y -= dy; // Invert Y
    
    // Clamp to screen bounds
    if (mouse_state.x < 0) mouse_state.x = 0;
    if (mouse_state.x >= VGA_WIDTH) mouse_state.x = VGA_WIDTH - 1;
    if (mouse_state.y < 0) mouse_state.y = 0;
    if (mouse_state.y >= VGA_HEIGHT) mouse_state.y = VGA_HEIGHT - 1;
}

MouseState* mouse_get_state() {
    return &mouse_state;
}

void mouse_draw_cursor(int x, int y, uint8_t color) {
    // Draw a simple arrow cursor (7x11 pixels)
    static const uint8_t cursor_data[11] = {
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11111100,
        0b11111110,
        0b11110000,
        0b11001000,
        0b10000100,
        0b00000100
    };
    
    for (int row = 0; row < 11; row++) {
        for (int col = 0; col < 8; col++) {
            if (cursor_data[row] & (1 << (7 - col))) {
                vga_put_pixel(x + col, y + row, color);
            }
        }
    }
}
