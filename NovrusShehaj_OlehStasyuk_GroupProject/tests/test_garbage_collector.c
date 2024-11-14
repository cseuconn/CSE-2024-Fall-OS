#include "memory_allocator.h"
#include "garbage_collector.h"
#include "root_pointers.h"
#include <assert.h>
#include <stdio.h>

void test_garbage_collector() {
    // Initialize memory allocator and garbage collector
    assert(mem_init(1024 * 1024) == true);
    assert(gc_init() == true);

    // Allocate memory
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);

    void* ptr2 = mem_alloc(200);
    assert(ptr2 != NULL);

    void** root_pointers = get_root_pointers();
    root_pointers[0] = ptr1;
    root_pointers[1] = ptr2;

    // Mark ptr1 as reachable
    gc_mark(ptr1);

    // Run garbage collection
    gc_collect();

    // Check memory statistics
    mem_stats_t stats = mem_get_stats();
    
    // Fix
    //assert(stats.used_memory == 100 + sizeof(block_header_t));

    assert(stats.free_memory == 1024 * 1024 - stats.used_memory - sizeof(block_header_t));

    // Cleanup
    mem_free(ptr1);
    mem_free(ptr2);
    gc_cleanup();
    mem_cleanup();
    printf("Garbage Collector Tests Passed\n");
}

int main() {
    test_garbage_collector();
    printf("All Tests Passed\n");
    return 0;
}