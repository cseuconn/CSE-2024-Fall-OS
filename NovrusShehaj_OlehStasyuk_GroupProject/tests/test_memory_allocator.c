#include "memory_allocator.h"
#include <assert.h>
#include <stdio.h>

void test_mem_init() {
    // Initialize memory allocator
    assert(mem_init(1024 * 1024) == true);

    // Check initial memory statistics
    mem_stats_t stats = mem_get_stats();
    assert(stats.total_memory == 1024 * 1024);
    assert(stats.used_memory == 0);
    assert(stats.free_memory == 1024 * 1024 - sizeof(block_header_t));
    assert(stats.total_allocation == 0);
    assert(stats.total_free == 0);
    assert(stats.fragments == 0);

    printf("test_mem_init passed.\n");
}

void test_mem_alloc_free() {
    // Allocate memory
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);

    void* ptr2 = mem_alloc(200);
    assert(ptr2 != NULL);

    // Check memory statistics after allocation
    mem_stats_t stats = mem_get_stats();
    assert(stats.used_memory != 0);
    assert(stats.free_memory != 1024 * 1024 - sizeof(block_header_t) - (100 + 200));
    assert(stats.total_allocation == 2);

    // Free memory
    mem_free(ptr1);
    mem_free(ptr2);

    // Check memory statistics after freeing
    stats = mem_get_stats();
    assert(stats.used_memory == 0);
    assert(stats.free_memory == 1024 * 1024 - sizeof(block_header_t));
    assert(stats.total_free == 2);

    printf("test_mem_alloc_free passed.\n");
}

void test_free_list_management() {
    // Allocate memory
    void* ptr1 = mem_alloc(100);
    void* ptr2 = mem_alloc(200);
    void* ptr3 = mem_alloc(300);

    // Free memory
    mem_free(ptr2);

    // Check that the free list contains the freed block
    block_header_t* free_block = allocator.free_list;
    assert(free_block != NULL);
    assert(free_block->size == 200);

    // Allocate memory again and check that the free block is reused
    void* ptr4 = mem_alloc(150);
    assert(ptr4 == ptr2); // ptr4 should be the same as ptr2

    printf("test_free_list_management passed.\n");
}

void test_coalescing() {
    // Allocate memory
    void* ptr1 = mem_alloc(100);
    void* ptr2 = mem_alloc(200);
    void* ptr3 = mem_alloc(300);

    // Free memory
    mem_free(ptr2);
    mem_free(ptr3);

    // Check that the free blocks are coalesced
    block_header_t* free_block = allocator.free_list;
    assert(free_block != NULL);

    printf("test_coalescing passed.\n");
}

void test_memory_allocator() {
    // Initialize memory allocator
    assert(mem_init(1024 * 1024) == true);

    // Allocate memory
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);

    void* ptr2 = mem_alloc(200);
    assert(ptr2 != NULL);

    // Free memory
    mem_free(ptr1);
    mem_free(ptr2);

    // Check memory statistics
    mem_stats_t stats = mem_get_stats();
    assert(stats.total_memory == 1024 * 1024);
    assert(stats.used_memory == 0);
    assert(stats.free_memory == 1024 * 1024 - sizeof(block_header_t));

    // Cleanup
    mem_cleanup();
    printf("Memory Allocator Tests Passed\n");
}

int main() {
    test_mem_init();
    test_mem_alloc_free();
    test_free_list_management();
    test_coalescing();
    test_memory_allocator();

    printf("All Tests Passed\n");
    return 0;
}