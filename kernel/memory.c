#include "../include/memory.h"

// Simple heap allocator
#define HEAP_START 0x100000  // 1 MB
#define HEAP_SIZE  0x100000  // 1 MB heap

static uint32_t heap_current = HEAP_START;
static uint32_t heap_end = HEAP_START + HEAP_SIZE;

void memory_init() {
    heap_current = HEAP_START;
}

void* kmalloc(size_t size) {
    // Align to 4 bytes
    size = (size + 3) & ~3;
    
    if (heap_current + size > heap_end) {
        return NULL;  // Out of memory
    }
    
    void* ptr = (void*)heap_current;
    heap_current += size;
    return ptr;
}

void kfree(void* ptr) {
    // Simple allocator doesn't support freeing
    // In a real OS, you'd implement a proper heap with free lists
    (void)ptr;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* ptr, int value, size_t n) {
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)value;
    }
    return ptr;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* p1 = (const uint8_t*)s1;
    const uint8_t* p2 = (const uint8_t*)s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}
