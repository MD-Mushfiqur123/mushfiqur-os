#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

// Memory management functions
void memory_init();
void* kmalloc(size_t size);
void kfree(void* ptr);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

#endif
