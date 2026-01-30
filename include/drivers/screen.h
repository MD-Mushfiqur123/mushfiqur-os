#ifndef SCREEN_H
#define SCREEN_H

#include "../types.h"

// Screen dimensions
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Screen colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

// Screen functions
void screen_clear();
void screen_write_char(char c, uint8_t color, int col, int row);
void screen_write_string(const char* str, uint8_t color, int col, int row);
void kprint(const char* str);
void kprint_color(const char* str, uint8_t color);
void kprint_at(const char* str, int col, int row);
void kprint_char(char c);
void kprint_backspace();
void screen_set_cursor(int offset);
int screen_get_cursor();
void screen_scroll();

#endif
