#include "memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define MIN_BLOCK_SIZE 32

memory_allocator_t allocator; // Define the allocator variable

static block_header_t* find_free_block(size_t size);
static void split_block(block_header_t* block, size_t size);
static void coalesce(block_header_t* block);
static void add_to_free_list(block_header_t* block);
static void remove_from_free_list(block_header_t* block);

bool mem_init(size_t pool_size) {
    printf("mem_init: Initializing memory pool of size %zu bytes\n", pool_size);
    allocator.pool_size = ALIGN(pool_size);
    allocator.memory_pool = malloc(allocator.pool_size);

    if (!allocator.memory_pool) {
        printf("mem_init: Failed to allocate memory pool\n");
        return false;
    }

    allocator.first_block = (block_header_t*)allocator.memory_pool;
    allocator.first_block->size = allocator.pool_size - sizeof(block_header_t);
    allocator.first_block->is_free = true;
    allocator.first_block->next = NULL;
    allocator.first_block->prev = NULL;
    allocator.first_block->next_free = NULL;

    allocator.free_list = allocator.first_block;

    allocator.stats.total_memory = allocator.pool_size;
    allocator.stats.used_memory = 0;
    allocator.stats.free_memory = allocator.first_block->size;
    allocator.stats.total_allocation = 0;
    allocator.stats.total_free = 0;
    allocator.stats.fragments = 0;

    printf("mem_init: Memory pool initialized successfully\n");
    return true;
}

static void split_block(block_header_t* block, size_t size) {
    if (block->size >= size + sizeof(block_header_t) + MIN_BLOCK_SIZE) {
        block_header_t* new_block = (block_header_t*)((char*)block + sizeof(block_header_t) + size);
        new_block->size = block->size - size - sizeof(block_header_t);
        new_block->is_free = true;
        new_block->next = block->next;
        new_block->prev = block;
        new_block->next_free = block->next_free;

        if (block->next) {
            block->next->prev = new_block;
        }

        block->next = new_block;
        block->size = size;

        add_to_free_list(new_block);
    }
}

void* mem_alloc(size_t size) {
    printf("mem_alloc: Requesting %zu bytes\n", size);
    size = ALIGN(size);
    block_header_t* block = allocator.free_list;

    while (block) {
        if (block->is_free && block->size >= size) {
            printf("mem_alloc: Found free block of size %zu\n", block->size);
            remove_from_free_list(block);
            if (block->size > size + sizeof(block_header_t) + MIN_BLOCK_SIZE) {
                split_block(block, size);
            }
            block->is_free = false;
            allocator.stats.used_memory += size; // Update used_memory with the requested size
            allocator.stats.free_memory -= size; // Update free_memory with the requested size
            allocator.stats.total_allocation++;
            return (void*)((char*)block + sizeof(block_header_t));
        }
        block = block->next_free;
    }

    printf("mem_alloc: No suitable free block found\n");
    return NULL;
}

static block_header_t* find_free_block(size_t size) {
    block_header_t* current = allocator.first_block;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void coalesce(block_header_t* block) {
    if (block->next && block->next->is_free) {
        block->size += block->next->size + sizeof(block_header_t);
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
}

/*
bool mem_init(size_t pool_size) {
    allocator.pool_size = ALIGN(pool_size);
    allocator.memory_pool = malloc(allocator.pool_size);

    if (!allocator.memory_pool) {
        return false;
    }

    allocator.first_block = (block_header_t*)allocator.memory_pool;
    allocator.first_block->size = allocator.pool_size - sizeof(block_header_t);
    allocator.first_block->is_free = true;
    allocator.first_block->next = NULL;
    allocator.first_block->prev = NULL;
    allocator.first_block->next_free = NULL;

    allocator.free_list = allocator.first_block;

    allocator.stats.total_memory = allocator.pool_size;
    allocator.stats.used_memory = 0;
    allocator.stats.free_memory = allocator.first_block->size;
    allocator.stats.total_allocation = 0;
    allocator.stats.total_free = 0;
    allocator.stats.fragments = 0;

    return true;
}
*/

static void add_to_free_list(block_header_t* block) {
    block->next_free = allocator.free_list;
    allocator.free_list = block;
}

static void remove_from_free_list(block_header_t* block) {
    block_header_t* current = allocator.free_list;
    block_header_t* prev = NULL;

    while (current) {
        if (current == block) {
            if (prev) {
                prev->next_free = current->next_free;
            } else {
                allocator.free_list = current->next_free;
            }
            break;
        }
        prev = current;
        current = current->next_free;
    }
}

/*
void* mem_alloc(size_t size) {
    size = ALIGN(size);
    block_header_t* block = allocator.free_list;

    while (block) {
        if (block->is_free && block->size >= size) {
            remove_from_free_list(block);
            if (block->size > size + sizeof(block_header_t) + MIN_BLOCK_SIZE) {
                split_block(block, size);
            }
            block->is_free = false;
            allocator.stats.used_memory += block->size;
            allocator.stats.free_memory -= block->size;
            allocator.stats.total_allocation++;
            return (void*)((char*)block + sizeof(block_header_t));
        }
        block = block->next_free;
    }

    return NULL;
}*/

void mem_free(void* ptr) {
    if (!ptr) {
        return;
    }

    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    block->is_free = true;
    allocator.stats.used_memory -= block->size;
    allocator.stats.free_memory += block->size;
    allocator.stats.total_free++;

    add_to_free_list(block);
    coalesce(block);
}

/*
void* mem_alloc(size_t size) {
    size = ALIGN(size);
    block_header_t* block = find_free_block(size);
    if (!block) {
        return NULL;
    }

    if (block->size > size + sizeof(block_header_t) + MIN_BLOCK_SIZE) {
        split_block(block, size);
    }

    block->is_free = false;
    allocator.stats.used_memory += block->size;
    allocator.stats.free_memory -= block->size;
    allocator.stats.total_allocation++;

    return (void*)((char*)block + sizeof(block_header_t));
}

void mem_free(void* ptr) {
    if (!ptr) {
        return;
    }

    block_header_t* block = (block_header_t*)((char*)ptr - sizeof(block_header_t));
    block->is_free = true;
    allocator.stats.used_memory -= block->size;
    allocator.stats.free_memory += block->size;
    allocator.stats.total_free++;

    coalesce(block);
}
*/

mem_stats_t mem_get_stats(void) {
    return allocator.stats;
}

void mem_dump_stats(void) {
    printf("Total Memory: %zu\n", allocator.stats.total_memory);
    printf("Used Memory: %zu\n", allocator.stats.used_memory);
    printf("Free Memory: %zu\n", allocator.stats.free_memory);
    printf("Total Allocations: %zu\n", allocator.stats.total_allocation);
    printf("Total Frees: %zu\n", allocator.stats.total_free);
    printf("Fragments: %zu\n", allocator.stats.fragments);
}

void mem_cleanup(void) {
    free(allocator.memory_pool);
    allocator.memory_pool = NULL;
    allocator.first_block = NULL;
    allocator.free_list = NULL;
}