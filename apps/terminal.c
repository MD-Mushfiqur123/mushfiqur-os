#include "../include/gui/window.h"
#include "../include/drivers/vga.h"

static char output_buffer[1000];
static int output_length = 0;
static char input_buffer[80];
static int input_pos = 0;

void terminal_render_content(int x, int y, int width, int height) {
    // Draw terminal background (black)
    vga_fill_rect(x, y, width, height, COLOR_BLACK);
    
    // Draw output text (green on black)
    int line = 0;
    int col = 0;
    for (int i = 0; i < output_length && line < (height / 8) - 1; i++) {
        if (output_buffer[i] == '\n') {
            line++;
            col = 0;
        } else {
            vga_draw_char(x + col * 8, y + line * 8, output_buffer[i], COLOR_LIGHT_GREEN);
            col++;
            if (col >= width / 8) {
                line++;
                col = 0;
            }
        }
    }
    
    // Draw input prompt
    int prompt_y = y + height - 16;
    vga_draw_string(x, prompt_y, "$", COLOR_LIGHT_GREEN);
    vga_draw_string(x + 8, prompt_y, input_buffer, COLOR_WHITE);
    
    // Draw cursor
    vga_fill_rect(x + 8 + input_pos * 8, prompt_y, 8, 8, COLOR_WHITE);
}

void terminal_add_output(const char* text) {
    while (*text && output_length < 999) {
        output_buffer[output_length++] = *text++;
    }
    output_buffer[output_length] = '\0';
}

void terminal_add_char(char c) {
    if (c == '\n') {
        // Execute command
        input_buffer[input_pos] = '\0';
        terminal_add_output("\n");
        terminal_add_output(input_buffer);
        terminal_add_output("\n");
        
        // Simple command handling
        if (input_buffer[0] == 'h' && input_buffer[1] == 'i') {
            terminal_add_output("Hello from GUI Terminal!\n");
        } else if (input_buffer[0] != '\0') {
            terminal_add_output("Command not found\n");
        }
        
        input_pos = 0;
        input_buffer[0] = '\0';
    } else if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            input_buffer[input_pos] = '\0';
        }
    } else if (input_pos < 79) {
        input_buffer[input_pos++] = c;
        input_buffer[input_pos] = '\0';
    }
}

Window* terminal_create() {
    output_length = 0;
    input_pos = 0;
    input_buffer[0] = '\0';
    
    terminal_add_output("Mushfiqur OS Terminal v1.0\n");
    terminal_add_output("Type 'hi' to test\n\n");
    
    Window* win = window_create(20, 30, 280, 140, "Terminal");
    win->render_content = terminal_render_content;
    win->bg_color = COLOR_BLACK;
    return win;
}
