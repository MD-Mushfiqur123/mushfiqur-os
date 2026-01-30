#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Special keys
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C
#define KEY_LSHIFT 0x2A
#define KEY_RSHIFT 0x36
#define KEY_LCTRL 0x1D
#define KEY_LALT 0x38

// Keyboard functions
void keyboard_init();
void keyboard_handler();
char keyboard_get_char();
void keyboard_set_callback(void (*callback)(char));

#endif
