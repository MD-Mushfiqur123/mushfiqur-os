#include "../include/drivers/screen.h"
#include "../include/kernel.h"

// Current cursor position
static int cursor_row = 0;
static int cursor_col = 0;

// Get offset from row and column
int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

// Get row from offset
int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

// Get column from offset
int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

// Set cursor position using VGA ports
void screen_set_cursor(int offset) {
    offset /= 2;
    outb(0x3d4, 14);
    outb(0x3d5, (uint8_t)(offset >> 8));
    outb(0x3d4, 15);
    outb(0x3d5, (uint8_t)(offset & 0xff));
}

// Get cursor position
int screen_get_cursor() {
    outb(0x3d4, 14);
    int offset = inb(0x3d5) << 8;
    outb(0x3d4, 15);
    offset += inb(0x3d5);
    return offset * 2;
}

// Clear screen
void screen_clear() {
    uint8_t* video = (uint8_t*)VIDEO_ADDRESS;
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = WHITE;
    }
    cursor_row = 0;
    cursor_col = 0;
    screen_set_cursor(get_offset(cursor_col, cursor_row));
}

// Scroll screen up by one line
void screen_scroll() {
    uint8_t* video = (uint8_t*)VIDEO_ADDRESS;
    
    // Copy each line to the previous line
    for (int i = 1; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            int src_offset = get_offset(j, i);
            int dst_offset = get_offset(j, i - 1);
            video[dst_offset] = video[src_offset];
            video[dst_offset + 1] = video[src_offset + 1];
        }
    }
    
    // Clear last line
    for (int j = 0; j < MAX_COLS; j++) {
        int offset = get_offset(j, MAX_ROWS - 1);
        video[offset] = ' ';
        video[offset + 1] = WHITE;
    }
    
    cursor_row = MAX_ROWS - 1;
}

// Write character at position
void screen_write_char(char c, uint8_t color, int col, int row) {
    uint8_t* video = (uint8_t*)VIDEO_ADDRESS;
    
    if (!color) color = WHITE;
    
    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    } else {
        offset = get_offset(cursor_col, cursor_row);
    }
    
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\r') {
        cursor_col = 0;
    } else if (c == '\t') {
        cursor_col = (cursor_col + 4) & ~(4 - 1);
    } else {
        video[offset] = c;
        video[offset + 1] = color;
        cursor_col++;
    }
    
    // Handle line wrapping
    if (cursor_col >= MAX_COLS) {
        cursor_col = 0;
        cursor_row++;
    }
    
    // Scroll if needed
    if (cursor_row >= MAX_ROWS) {
        screen_scroll();
    }
    
    screen_set_cursor(get_offset(cursor_col, cursor_row));
}

// Print string at current cursor position
void kprint(const char* str) {
    kprint_color(str, WHITE);
}

// Print string with color
void kprint_color(const char* str, uint8_t color) {
    int i = 0;
    while (str[i] != '\0') {
        screen_write_char(str[i], color, -1, -1);
        i++;
    }
}

// Print string at specific position
void kprint_at(const char* str, int col, int row) {
    cursor_col = col;
    cursor_row = row;
    kprint(str);
}

// Print single character
void kprint_char(char c) {
    screen_write_char(c, WHITE, -1, -1);
}

// Handle backspace
void kprint_backspace() {
    if (cursor_col > 0) {
        cursor_col--;
        screen_write_char(' ', WHITE, cursor_col, cursor_row);
        cursor_col--;
        screen_write_char(' ', WHITE, -1, -1);
        cursor_col--;
    }
}
