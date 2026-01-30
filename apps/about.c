#include "../include/gui/window.h"
#include "../include/drivers/vga.h"

void about_render_content(int x, int y, int width, int height) {
    // Draw about dialog content
    vga_fill_rect(x, y, width, height, COLOR_WHITE);
    
    int cy = y + 10;
    vga_draw_string(x + 10, cy, "MUSHFIQUR OS", COLOR_BLUE);
    cy += 16;
    vga_draw_string(x + 10, cy, "Version 1.0.0", COLOR_BLACK);
    cy += 12;
    vga_draw_string(x + 10, cy, "Genesis Edition", COLOR_BLACK);
    cy += 16;
    vga_draw_string(x + 10, cy, "32-bit OS", COLOR_DARK_GRAY);
    cy += 12;
    vga_draw_string(x + 10, cy, "with GUI", COLOR_DARK_GRAY);
    cy += 16;
    vga_draw_string(x + 10, cy, "Created by:", COLOR_BLACK);
    cy += 12;
    vga_draw_string(x + 10, cy, "Mushfiqur", COLOR_BLUE);
}

Window* about_create() {
    Window* win = window_create(60, 50, 200, 120, "About");
    win->render_content = about_render_content;
    win->bg_color = COLOR_WHITE;
    return win;
}
