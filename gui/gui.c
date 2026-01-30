#include <stdbool.h>
#include "../include/gui/gui.h"
#include "../include/gui/window.h"
#include "../include/gui/desktop.h"
#include "../include/drivers/vga.h"
#include "../include/drivers/mouse.h"
#include "../include/drivers/keyboard.h"

// Forward declarations from apps
extern Window* terminal_create();
extern Window* about_create();
extern Window* calculator_create();
extern void terminal_add_char(char c);

static bool gui_running = false;
static Window* dragging_window = NULL;
static int drag_offset_x = 0;
static int drag_offset_y = 0;
static Window* active_terminal = NULL;

void launch_terminal() {
    active_terminal = terminal_create();
}

void launch_about() {
    about_create();
}

void launch_calculator() {
    calculator_create();
}

void gui_init() {
    // Initialize VGA graphics mode
    vga_init();
    vga_set_mode_13h();
    
    // Initialize subsystems
    window_init();
    desktop_init();
    mouse_init();
    
    // Add desktop icons
    desktop_add_icon(10, 10, "Terminal", launch_terminal);
    desktop_add_icon(10, 60, "About", launch_about);
    desktop_add_icon(10, 110, "Calc", launch_calculator);
    
    gui_running = true;
}

void gui_handle_mouse() {
    static bool was_pressed = false;
    MouseState* mouse = mouse_get_state();
    
    if (mouse->left_button && !was_pressed) {
        // Mouse button just pressed
        Window* win = window_get_at_position(mouse->x, mouse->y);
        
        if (win) {
            // Check if clicked on title bar (for dragging)
            if (mouse->y >= win->y && mouse->y < win->y + TITLE_BAR_HEIGHT) {
                // Check if clicked close button
                int close_x = win->x + win->width - 10;
                if (mouse->x >= close_x && mouse->x < close_x + 8) {
                    window_destroy(win);
                    if (win == active_terminal) active_terminal = NULL;
                } else {
                    // Start dragging
                    dragging_window = win;
                    drag_offset_x = mouse->x - win->x;
                    drag_offset_y = mouse->y - win->y;
                    window_set_focus(win);
                }
            } else {
                window_set_focus(win);
            }
        } else {
            // Clicked on desktop
            desktop_handle_click(mouse->x, mouse->y);
        }
        
        was_pressed = true;
    } else if (!mouse->left_button && was_pressed) {
        // Mouse button released
        dragging_window = NULL;
        was_pressed = false;
    }
    
    // Handle dragging
    if (dragging_window && mouse->left_button) {
        window_move(dragging_window, 
                   mouse->x - drag_offset_x,
                   mouse->y - drag_offset_y);
    }
}

void gui_handle_keyboard(char c) {
    // Send keyboard input to active terminal
    if (active_terminal && active_terminal->visible) {
        terminal_add_char(c);
    }
}

void gui_run() {
    while (gui_running) {
        // Handle mouse input
        mouse_handler();
        gui_handle_mouse();
        
        // Render everything
        desktop_render();
        window_render_all();
        
        // Draw mouse cursor
        MouseState* mouse = mouse_get_state();
        mouse_draw_cursor(mouse->x, mouse->y, COLOR_WHITE);
        
        // Small delay to prevent excessive CPU usage
        for (volatile int i = 0; i < 10000; i++);
    }
}
