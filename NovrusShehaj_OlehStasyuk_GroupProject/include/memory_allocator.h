#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>


// Forward declaration of block_header_t
typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
    struct block_header* prev;
    struct block_header* next_free; // Pointer to the next free block
} block_header_t;

// Statistics Structure
typedef struct {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;
    size_t total_allocation;
    size_t total_free;
    size_t fragments;
} mem_stats_t;

typedef struct {
    block_header_t* first_block;
    block_header_t* free_list; // Pointer to the first free block
    mem_stats_t stats; // Add the stats field
    void* memory_pool;
    size_t pool_size;
} memory_allocator_t;

// Declare the allocator variable as extern
extern memory_allocator_t allocator;
//void* mem_alloc(size_t size);

// Initialize Memory Allocator
bool mem_init(size_t pool_size);

// Core Allocation Functions
void* mem_alloc(size_t size);
void mem_free(void* ptr);

// Utility Functions
mem_stats_t mem_get_stats(void);
void mem_dump_stats(void);
void mem_cleanup(void);

#endif // MEMORY_ALLOCATOR_H