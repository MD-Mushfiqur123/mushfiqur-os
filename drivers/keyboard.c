#include "../include/drivers/keyboard.h"
#include "../include/drivers/screen.h"
#include "../include/kernel.h"

// Scancode to ASCII mapping (US keyboard layout)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static const char scancode_to_ascii_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static bool shift_pressed = FALSE;
static void (*key_callback)(char) = NULL;

void keyboard_init() {
    // Keyboard initialization
    // In a full implementation, we would set up IDT and enable IRQ1
}

void keyboard_set_callback(void (*callback)(char)) {
    key_callback = callback;
}

void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Handle key release (scancode with bit 7 set)
    if (scancode & 0x80) {
        scancode &= 0x7F;
        if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
            shift_pressed = FALSE;
        }
        return;
    }
    
    // Handle special keys
    if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
        shift_pressed = TRUE;
        return;
    }
    
    if (scancode == KEY_BACKSPACE) {
        if (key_callback) {
            key_callback('\b');
        }
        return;
    }
    
    if (scancode == KEY_ENTER) {
        if (key_callback) {
            key_callback('\n');
        }
        return;
    }
    
    // Convert scancode to ASCII
    char ascii = 0;
    if (scancode < sizeof(scancode_to_ascii)) {
        if (shift_pressed) {
            ascii = scancode_to_ascii_shift[scancode];
        } else {
            ascii = scancode_to_ascii[scancode];
        }
    }
    
    if (ascii && key_callback) {
        key_callback(ascii);
    }
}

char keyboard_get_char() {
    // Wait for keypress
    while (1) {
        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if (status & 0x01) {
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            
            if (scancode & 0x80) {
                scancode &= 0x7F;
                if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
                    shift_pressed = FALSE;
                }
                continue;
            }
            
            if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
                shift_pressed = TRUE;
                continue;
            }
            
            if (scancode == KEY_BACKSPACE) {
                return '\b';
            }
            
            if (scancode == KEY_ENTER) {
                return '\n';
            }
            
            char ascii = 0;
            if (scancode < sizeof(scancode_to_ascii)) {
                if (shift_pressed) {
                    ascii = scancode_to_ascii_shift[scancode];
                } else {
                    ascii = scancode_to_ascii[scancode];
                }
            }
            
            if (ascii) {
                return ascii;
            }
        }
    }
}
