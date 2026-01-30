#include "../include/drivers/screen.h"
#include "../include/drivers/keyboard.h"
#include "../include/kernel.h"

// String library functions
int strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

void strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int strncmp(const char* s1, const char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        if (s1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

// Shell state
#define MAX_INPUT 256
static char input_buffer[MAX_INPUT];
static int input_pos = 0;

// Command handlers
void cmd_help() {
    kprint_color("\nAvailable commands:\n", LIGHT_CYAN);
    kprint("  help      - Show this help message\n");
    kprint("  about     - Display OS information\n");
    kprint("  clear     - Clear the screen\n");
    kprint("  echo      - Echo text to screen\n");
    kprint("  version   - Show OS version\n");
    kprint("  shutdown  - Halt the system\n");
    kprint("\n");
}

void cmd_about() {
    kprint("\n");
    kprint_color("╔════════════════════════════════════════════════════════════════╗\n", LIGHT_CYAN);
    kprint_color("║                      MUSHFIQUR OS                              ║\n", YELLOW);
    kprint_color("╠════════════════════════════════════════════════════════════════╣\n", LIGHT_CYAN);
    kprint("║  Version:     1.0.0 Genesis                                    ║\n");
    kprint("║  Type:        32-bit Protected Mode Operating System          ║\n");
    kprint("║  Kernel:      Monolithic                                       ║\n");
    kprint("║  Architecture: x86 (i686)                                      ║\n");
    kprint("║  Author:      Mushfiqur Rahman                                 ║\n");
    kprint_color("╚════════════════════════════════════════════════════════════════╝\n", LIGHT_CYAN);
    kprint("\n");
}

void cmd_version() {
    kprint("\n");
    kprint_color(OS_NAME, YELLOW);
    kprint(" version ");
    kprint_color(OS_VERSION, LIGHT_GREEN);
    kprint(" (");
    kprint_color(OS_CODENAME, LIGHT_CYAN);
    kprint(")\n\n");
}

void cmd_clear() {
    screen_clear();
}

void cmd_echo(const char* args) {
    kprint("\n");
    if (args && strlen(args) > 0) {
        kprint(args);
    }
    kprint("\n");
}

void cmd_shutdown() {
    kprint("\n");
    kprint_color("Shutting down Mushfiqur OS...\n", LIGHT_RED);
    kprint("System halted. You can now power off.\n");
    halt();
}

// Process command
void process_command(const char* cmd) {
    if (strlen(cmd) == 0) {
        return;
    }
    
    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "about") == 0) {
        cmd_about();
    } else if (strcmp(cmd, "version") == 0) {
        cmd_version();
    } else if (strcmp(cmd, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(cmd, "shutdown") == 0) {
        cmd_shutdown();
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        cmd_echo(cmd + 5);
    } else {
        kprint("\n");
        kprint_color("Unknown command: ", LIGHT_RED);
        kprint(cmd);
        kprint("\nType 'help' for available commands.\n");
    }
}

// Show prompt
void show_prompt() {
    kprint_color("mushfiqur", LIGHT_GREEN);
    kprint_color("@", WHITE);
    kprint_color("os", LIGHT_CYAN);
    kprint_color(":~$ ", WHITE);
}

// Handle keyboard input
void shell_key_handler(char c) {
    if (c == '\n') {
        kprint_char('\n');
        input_buffer[input_pos] = '\0';
        process_command(input_buffer);
        input_pos = 0;
        show_prompt();
    } else if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            kprint_backspace();
        }
    } else {
        if (input_pos < MAX_INPUT - 1) {
            input_buffer[input_pos++] = c;
            kprint_char(c);
        }
    }
}

// Initialize shell
void shell_init() {
    kprint_color("Starting Mushfiqur Shell...\n", LIGHT_CYAN);
    kprint("Type 'help' for available commands.\n\n");
    
    keyboard_set_callback(shell_key_handler);
    show_prompt();
    
    // Main shell loop
    while (1) {
        keyboard_handler();
    }
}
