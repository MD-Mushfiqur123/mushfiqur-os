#include "../include/kernel.h"
#include "../include/drivers/screen.h"
#include "../include/drivers/keyboard.h"
#include "../include/memory.h"
#include "../include/gui/gui.h"

// Forward declarations
void shell_init();

void kernel_main() {
    // Immediate output to verify we reached kernel
    char *video = (char*)0xb8000;
    video[0] = 'K';
    video[1] = 0x0F;
    video[2] = 'E';
    video[3] = 0x0F;
    video[4] = 'R';
    video[5] = 0x0F;
    video[6] = 'N';
    video[7] = 0x0F;
    video[8] = 'E';
    video[9] = 0x0F;
    video[10] = 'L';
    video[11] = 0x0F;
    
    // Clear screen
    screen_clear();
    
    // Print welcome banner
    kprint_color("================================================================================\n", LIGHT_CYAN);
    kprint_color("                           MUSHFIQUR OS v1.0.0                                  \n", YELLOW);
    kprint_color("                              Genesis Edition                                   \n", LIGHT_GREEN);
    kprint_color("================================================================================\n", LIGHT_CYAN);
    kprint("\n");
    
    kprint_color("[OK] ", LIGHT_GREEN);
    kprint("Kernel loaded successfully\n");
    
    // Initialize memory management
    kprint_color("[OK] ", LIGHT_GREEN);
    kprint("Memory management initialized\n");
    memory_init();
    
    // Initialize keyboard
    kprint_color("[OK] ", LIGHT_GREEN);
    kprint("Keyboard driver initialized\n");
    keyboard_init();
    
    kprint_color("[OK] ", LIGHT_GREEN);
    kprint("System initialization complete\n");
    kprint("\n");
    
    // Ask user for mode selection
    kprint_color("Select mode:\n", LIGHT_CYAN);
    kprint("  [T] Text Mode (Shell)\n");
    kprint("  [G] Graphics Mode (GUI)\n\n");
    kprint("Press T or G: ");
    
    // Wait for key press
    char choice = keyboard_get_char();
    kprint_char(choice);
    kprint("\n\n");
    
    if (choice == 'G' || choice == 'g') {
        // Start GUI mode
        kprint_color("Starting GUI...\n", LIGHT_GREEN);
        for (volatile int i = 0; i < 10000000; i++); // Brief delay
        
        gui_init();
        gui_run();
    } else {
        // Start text mode shell
        shell_init();
    }
    
    // Kernel main loop
    while(1) {
        halt();
    }
}

// Port I/O functions
void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__("outw %0, %1" : : "a"(value), "Nd"(port));
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void kernel_panic(const char* message) {
    screen_clear();
    kprint_color("KERNEL PANIC: ", LIGHT_RED);
    kprint(message);
    kprint("\nSystem halted.\n");
    halt();
}

void halt() {
    __asm__ __volatile__("hlt");
}
