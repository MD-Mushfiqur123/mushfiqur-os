#ifndef GUI_H
#define GUI_H

#include "../types.h"

// GUI main functions
void gui_init();
void gui_run();
void gui_handle_mouse();
void gui_handle_keyboard(char c);

// Application launchers
void launch_terminal();
void launch_about();
void launch_calculator();

#endif
