// root_pointers.c
#include <stddef.h>
#include <stdbool.h>
#include "root_pointers.h"
#include "memory_allocator.h"

static block_header_t block1 = { .size = 64, .is_free = false, .next = NULL, .prev = NULL };
static block_header_t block2 = { .size = 128, .is_free = false, .next = NULL, .prev = NULL };

static void* root_pointers[] = {
    // Add your root pointers here
    (void*)((char*)&block1 + sizeof(block_header_t)),
    (void*)((char*)&block2 + sizeof(block_header_t))
};

static size_t num_root_pointers = sizeof(root_pointers) / sizeof(root_pointers[0]);

size_t get_num_root_pointers() {
    return num_root_pointers;
}

void** get_root_pointers() {
    return root_pointers;
}