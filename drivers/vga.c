#include "../include/drivers/vga.h"
#include "../include/kernel.h"

static uint8_t* vga_buffer = (uint8_t*)VGA_MEMORY;

// 8x8 bitmap font (simplified - only basic ASCII)
static uint8_t font_8x8[128][8] = {
    // Space (32)
    [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // ! (33)
    [33] = {0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},
    // A (65)
    [65] = {0x18, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00},
    // B (66)
    [66] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00},
    // C (67)
    [67] = {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00},
};

void vga_write_registers(uint8_t *regs) {
    unsigned i;
    
    // Write MISCELLANEOUS register
    outb(VGA_MISC_WRITE, *regs);
    regs++;
    
    // Write SEQUENCER registers
    for(i = 0; i < 5; i++) {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *regs);
        regs++;
    }
    
    // Unlock CRTC registers
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
    
    // Write CRTC registers
    for(i = 0; i < 25; i++) {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *regs);
        regs++;
    }
    
    // Write GRAPHICS CONTROLLER registers
    for(i = 0; i < 9; i++) {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *regs);
        regs++;
    }
    
    // Write ATTRIBUTE CONTROLLER registers
    for(i = 0; i < 21; i++) {
        (void)inb(0x3DA);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *regs);
        regs++;
    }
    
    // Lock 16-color palette and unblank display
    (void)inb(0x3DA);
    outb(VGA_AC_INDEX, 0x20);
}

void vga_init() {
    // Initialize font data for common characters
    for (int i = 0; i < 128; i++) {
        if (font_8x8[i][0] == 0 && i >= 33) {
            // Default pattern for undefined characters
            for (int j = 0; j < 8; j++) {
                font_8x8[i][j] = 0x7E;
            }
        }
    }
}

void vga_set_mode_13h() {
    // VGA Mode 13h register values
    static uint8_t mode13h_regs[] = {
        // MISC
        0x63,
        // SEQ
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        // CRTC
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
        0xFF,
        // GC
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        // AC
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00, 0x00
    };
    
    vga_write_registers(mode13h_regs);
    vga_buffer = (uint8_t*)VGA_MEMORY;
}

void vga_set_text_mode() {
    // VGA Text Mode 3 register values  
    static uint8_t text_mode_regs[] = {
        // MISC
        0x67,
        // SEQ
        0x03, 0x00, 0x03, 0x00, 0x02,
        // CRTC
        0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
        0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
        0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
        0xFF,
        // GC
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
        0xFF,
        // AC
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x0C, 0x00, 0x0F, 0x08, 0x00
    };
    
    vga_write_registers(text_mode_regs);
}

void vga_put_pixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        vga_buffer[y * VGA_WIDTH + x] = color;
    }
}

void vga_clear_screen(uint8_t color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = color;
    }
}

void vga_draw_line(int x1, int y1, int x2, int y2, uint8_t color) {
    // Bresenham's line algorithm
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        vga_put_pixel(x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void vga_draw_rect(int x, int y, int width, int height, uint8_t color) {
    // Draw rectangle outline
    vga_draw_line(x, y, x + width - 1, y, color);                    // Top
    vga_draw_line(x, y + height - 1, x + width - 1, y + height - 1, color); // Bottom
    vga_draw_line(x, y, x, y + height - 1, color);                   // Left
    vga_draw_line(x + width - 1, y, x + width - 1, y + height - 1, color);  // Right
}

void vga_fill_rect(int x, int y, int width, int height, uint8_t color) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            vga_put_pixel(x + i, y + j, color);
        }
    }
}

void vga_draw_char(int x, int y, char c, uint8_t color) {
    uint8_t* glyph = font_8x8[(int)c];
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << (7 - col))) {
                vga_put_pixel(x + col, y + row, color);
            }
        }
    }
}

void vga_draw_string(int x, int y, const char* str, uint8_t color) {
    int offset = 0;
    while (*str) {
        vga_draw_char(x + offset, y, *str, color);
        offset += 8;
        str++;
    }
}
