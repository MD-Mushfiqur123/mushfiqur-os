#ifndef MOUSE_H
#define MOUSE_H

#include "../types.h"

// Mouse packet structure
typedef struct {
    int8_t x_movement;
    int8_t y_movement;
    uint8_t buttons;
} MousePacket;

// Mouse state
typedef struct {
    int x;
    int y;
    bool left_button;
    bool right_button;
    bool middle_button;
} MouseState;

// Mouse functions
void mouse_init();
void mouse_handler();
MouseState* mouse_get_state();
void mouse_draw_cursor(int x, int y, uint8_t color);

#endif
