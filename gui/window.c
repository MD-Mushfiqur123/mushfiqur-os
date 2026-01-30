#include <stdbool.h>
#include "../include/gui/window.h"
#include "../include/drivers/vga.h"
#include "../include/memory.h"

static Window windows[MAX_WINDOWS];
static int window_count = 0;
static Window* focused_window = NULL;

void window_init() {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        windows[i].visible = false;
    }
    window_count = 0;
}

Window* window_create(int x, int y, int width, int height, const char* title) {
    if (window_count >= MAX_WINDOWS) return NULL;
    
    Window* win = &windows[window_count++];
    win->x = x;
    win->y = y;
    win->width = width;
    win->height = height;
    win->visible = true;
    win->focused = false;
    win->bg_color = COLOR_LIGHT_GRAY;
    win->border_color = COLOR_DARK_GRAY;
    win->render_content = NULL;
    
    // Copy title
    int i = 0;
    while (title[i] && i < 63) {
        win->title[i] = title[i];
        i++;
    }
    win->title[i] = '\0';
    
    window_set_focus(win);
    return win;
}

void window_destroy(Window* win) {
    win->visible = false;
    if (focused_window == win) {
        focused_window = NULL;
    }
}

void window_render(Window* win) {
    if (!win->visible) return;
    
    // Draw title bar
    uint8_t title_color = win->focused ? COLOR_BLUE : COLOR_DARK_GRAY;
    vga_fill_rect(win->x, win->y, win->width, TITLE_BAR_HEIGHT, title_color);
    
    // Draw title text
    vga_draw_string(win->x + 4, win->y + 2, win->title, COLOR_WHITE);
    
    // Draw close button (X)
    int close_x = win->x + win->width - 10;
    int close_y = win->y + 2;
    vga_fill_rect(close_x, close_y, 8, 8, COLOR_RED);
    vga_draw_string(close_x + 1, close_y, "X", COLOR_WHITE);
    
    // Draw window border
    vga_draw_rect(win->x, win->y, win->width, win->height, win->border_color);
    
    // Draw content area background
    vga_fill_rect(win->x + BORDER_WIDTH, 
                  win->y + TITLE_BAR_HEIGHT, 
                  win->width - 2 * BORDER_WIDTH, 
                  win->height - TITLE_BAR_HEIGHT - BORDER_WIDTH,
                  win->bg_color);
    
    // Render custom content if callback is set
    if (win->render_content) {
        win->render_content(win->x + BORDER_WIDTH, 
                           win->y + TITLE_BAR_HEIGHT,
                           win->width - 2 * BORDER_WIDTH,
                           win->height - TITLE_BAR_HEIGHT - BORDER_WIDTH);
    }
}

void window_render_all() {
    for (int i = 0; i < window_count; i++) {
        if (windows[i].visible) {
            window_render(&windows[i]);
        }
    }
}

void window_set_focus(Window* win) {
    if (focused_window) {
        focused_window->focused = false;
    }
    focused_window = win;
    if (win) {
        win->focused = true;
    }
}

Window* window_get_at_position(int x, int y) {
    // Check from top to bottom (reverse order)
    for (int i = window_count - 1; i >= 0; i--) {
        Window* win = &windows[i];
        if (win->visible &&
            x >= win->x && x < win->x + win->width &&
            y >= win->y && y < win->y + win->height) {
            return win;
        }
    }
    return NULL;
}

void window_move(Window* win, int new_x, int new_y) {
    win->x = new_x;
    win->y = new_y;
    
    // Clamp to screen bounds
    if (win->x < 0) win->x = 0;
    if (win->y < 0) win->y = 0;
    if (win->x + win->width > VGA_WIDTH) win->x = VGA_WIDTH - win->width;
    if (win->y + win->height > VGA_HEIGHT) win->y = VGA_HEIGHT - win->height;
}
