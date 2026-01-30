#include "../include/gui/desktop.h"
#include "../include/gui/window.h"
#include "../include/drivers/vga.h"

#define MAX_ICONS 10

static DesktopIcon icons[MAX_ICONS];
static int icon_count = 0;

void desktop_init() {
    icon_count = 0;
}

void desktop_render() {
    // Draw desktop background (gradient effect)
    for (int y = 0; y < VGA_HEIGHT - TASKBAR_HEIGHT; y++) {
        uint8_t color = COLOR_BLUE + (y / 40);
        if (color > COLOR_LIGHT_BLUE) color = COLOR_LIGHT_BLUE;
        vga_fill_rect(0, y, VGA_WIDTH, 1, color);
    }
    
    // Draw taskbar at bottom
    vga_fill_rect(0, VGA_HEIGHT - TASKBAR_HEIGHT, VGA_WIDTH, TASKBAR_HEIGHT, COLOR_DARK_GRAY);
    
    // Draw "Start" button
    vga_fill_rect(2, VGA_HEIGHT - TASKBAR_HEIGHT + 2, 40, 12, COLOR_LIGHT_GRAY);
    vga_draw_rect(2, VGA_HEIGHT - TASKBAR_HEIGHT + 2, 40, 12, COLOR_BLACK);
    vga_draw_string(6, VGA_HEIGHT - TASKBAR_HEIGHT + 4, "Start", COLOR_BLACK);
    
    // Draw system tray (clock placeholder)
    vga_draw_string(VGA_WIDTH - 40, VGA_HEIGHT - TASKBAR_HEIGHT + 4, "12:00", COLOR_WHITE);
    
    // Draw desktop icons
    for (int i = 0; i < icon_count; i++) {
        DesktopIcon* icon = &icons[i];
        
        // Draw icon background
        vga_fill_rect(icon->x, icon->y, ICON_SIZE, ICON_SIZE, COLOR_LIGHT_BLUE);
        vga_draw_rect(icon->x, icon->y, ICON_SIZE, ICON_SIZE, COLOR_WHITE);
        
        // Draw icon symbol (simple square)
        vga_fill_rect(icon->x + 8, icon->y + 4, 16, 16, COLOR_YELLOW);
        
        // Draw label below icon
        vga_draw_string(icon->x, icon->y + ICON_SIZE + 2, icon->label, COLOR_WHITE);
    }
}

void desktop_add_icon(int x, int y, const char* label, void (*on_click)()) {
    if (icon_count >= MAX_ICONS) return;
    
    DesktopIcon* icon = &icons[icon_count++];
    icon->x = x;
    icon->y = y;
    icon->on_click = on_click;
    
    // Copy label
    int i = 0;
    while (label[i] && i < 31) {
        icon->label[i] = label[i];
        i++;
    }
    icon->label[i] = '\0';
}

void desktop_handle_click(int x, int y) {
    // Check if clicked on an icon
    for (int i = 0; i < icon_count; i++) {
        DesktopIcon* icon = &icons[i];
        if (x >= icon->x && x < icon->x + ICON_SIZE &&
            y >= icon->y && y < icon->y + ICON_SIZE + 10) {
            if (icon->on_click) {
                icon->on_click();
            }
            return;
        }
    }
}
