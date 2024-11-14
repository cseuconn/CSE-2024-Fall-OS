#include "defragmenter.h"
#include "memory_allocator.h"

// Assuming memory_allocator_t is defined in memory_allocator.h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
    struct block_header* prev;
} block_header_t;
*/

static void move_block(block_header_t* dest, block_header_t* src);

bool defragmenter_init(void) {
    // Initialize any necessary data structures
    return true;
}

extern memory_allocator_t allocator;

void defragmenter_run(void) {
    block_header_t* current = allocator.first_block;
    block_header_t* next_free = NULL;

    while (current) {
        if (current->is_free) {
            if (!next_free) {
                next_free = current;
            }
        } else {
            if (next_free) {
                move_block(next_free, current);
                next_free = (block_header_t*)((char*)next_free + sizeof(block_header_t) + next_free->size);
            }
        }
        current = current->next;
    }
}

static void move_block(block_header_t* dest, block_header_t* src) {
    // Move the memory block from src to dest
    size_t block_size = src->size;
    memmove((char*)dest + sizeof(block_header_t), (char*)src + sizeof(block_header_t), block_size);

    // Update the block header information
    dest->size = block_size;
    dest->is_free = false;
    dest->next = src->next;
    dest->prev = src->prev;

    if (dest->next) {
        dest->next->prev = dest;
    }

    if (dest->prev) {
        dest->prev->next = dest;
    }

    // Mark the source block as free
    src->is_free = true;
}

void  defragmenter_cleanup(void) {
    // Clean up any allocated resources
}