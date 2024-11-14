#include "garbage_collector.h"
#include "memory_allocator.h"
#include "root_pointers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Declare external functions to get root pointers
extern void** get_root_pointers(void);
extern size_t get_num_root_pointers(void);

static void mark(block_header_t* block);
static void sweep(void);

bool gc_init(void) {
    // Initialize any necessary data structures or state
    return true;
}

void gc_mark(void* ptr) {
    if (!ptr) return;
    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    printf("gc_mark: ptr=%p, block=%p\n", ptr, block);
    mark(block);
}

static void mark(block_header_t* block) {
    if (!block) {
        printf("mark: block is NULL\n");
        return;
    }
    if (!block->is_free) {
        printf("mark: block=%p is not free\n", block);
        return;
    }
    block->is_free = false; // Mark the block as reachable
    // Recursively mark any other reachable blocks
}

void gc_collect(void) {
    printf("Starting garbage collection...\n");
    // Mark phase: mark all reachable blocks
    // This would typically involve traversing all root pointers and marking reachable blocks
    void** roots = get_root_pointers();
    size_t num_roots = get_num_root_pointers();
    printf("Number of root pointers: %zu\n", num_roots);

    for (size_t i = 0; i < num_roots; i++) {
        printf("Marking root pointer %zu: %p\n", i, roots[i]);
        gc_mark(roots[i]);
    }

    // Sweep phase: free all unreachable blocks
    sweep();

    printf("Garbage collection completed.\n");
}

static void sweep(void) {
    block_header_t* current = allocator.first_block;
    while (current) {
        if (current->is_free) {
            mem_free((void*)((char*)current + sizeof(block_header_t)));
        }
        current = current->next;
    }
}

void gc_cleanup(void) {
    // Cleanup any allocated resources or state
}