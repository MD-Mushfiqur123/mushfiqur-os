#ifndef WINDOW_H
#define WINDOW_H

#include "../types.h"

#define MAX_WINDOWS 10
#define TITLE_BAR_HEIGHT 12
#define BORDER_WIDTH 1

typedef struct {
    int x, y;
    int width, height;
    char title[64];
    bool visible;
    bool focused;
    uint8_t bg_color;
    uint8_t border_color;
    void (*render_content)(int x, int y, int width, int height);
} Window;

// Window manager functions
void window_init();
Window* window_create(int x, int y, int width, int height, const char* title);
void window_destroy(Window* win);
void window_render(Window* win);
void window_render_all();
void window_set_focus(Window* win);
Window* window_get_at_position(int x, int y);
void window_move(Window* win, int new_x, int new_y);

#endif
