#include "memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define MIN_BLOCK_SIZE 32

memory_allocator_t allocator; // Define the allocator variable

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

static void split_block(block_header_t* block, size_t size) {
    if (block->size >= size + sizeof(block_header_t) + MIN_BLOCK_SIZE) {
        block_header_t* new_block = (block_header_t*)((char*)block + sizeof(block_header_t) + size);
        new_block->size = block->size - size - sizeof(block_header_t);
        new_block->is_free = true;
        new_block->next = block->next;
        new_block->prev = block;
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;
        block->size = size;
    }
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

    allocator.stats.total_memory = allocator.pool_size;
    allocator.stats.used_memory = 0;
    allocator.stats.free_memory = allocator.first_block->size;
    allocator.stats.total_allocation = 0;
    allocator.stats.total_free = 0;
    allocator.stats.fragments = 0;

    return true;
}

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
}