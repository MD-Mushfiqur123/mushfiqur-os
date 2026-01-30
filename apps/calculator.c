#include "../include/gui/window.h"
#include "../include/drivers/vga.h"

static char display[16] = "0";
static int display_value = 0;

void calculator_render_content(int x, int y, int width, int height) {
    // Draw calculator display
    vga_fill_rect(x + 5, y + 5, width - 10, 20, COLOR_WHITE);
    vga_draw_rect(x + 5, y + 5, width - 10, 20, COLOR_BLACK);
    vga_draw_string(x + 10, y + 10, display, COLOR_BLACK);
    
    // Draw buttons (simplified layout)
    const char* buttons[] = {"7", "8", "9", "+",
                             "4", "5", "6", "-",
                             "1", "2", "3", "*",
                             "C", "0", "=", "/"};
    
    int btn_width = 30;
    int btn_height = 20;
    int start_y = y + 30;
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int btn_x = x + 5 + col * (btn_width + 5);
            int btn_y = start_y + row * (btn_height + 5);
            
            vga_fill_rect(btn_x, btn_y, btn_width, btn_height, COLOR_LIGHT_GRAY);
            vga_draw_rect(btn_x, btn_y, btn_width, btn_height, COLOR_BLACK);
            vga_draw_string(btn_x + 10, btn_y + 6, buttons[row * 4 + col], COLOR_BLACK);
        }
    }
}

Window* calculator_create() {
    display[0] = '0';
    display[1] = '\0';
    display_value = 0;
    
    Window* win = window_create(80, 40, 150, 140, "Calculator");
    win->render_content = calculator_render_content;
    win->bg_color = COLOR_LIGHT_GRAY;
    return win;
}
