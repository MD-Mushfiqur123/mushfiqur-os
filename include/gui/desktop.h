#ifndef DESKTOP_H
#define DESKTOP_H

#include "../types.h"

#define TASKBAR_HEIGHT 16
#define ICON_SIZE 32

typedef struct {
    int x, y;
    char label[32];
    void (*on_click)();
} DesktopIcon;

// Desktop functions
void desktop_init();
void desktop_render();
void desktop_add_icon(int x, int y, const char* label, void (*on_click)());
void desktop_handle_click(int x, int y);

#endif
