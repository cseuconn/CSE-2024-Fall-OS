#include "memory_allocator.h"
#include <assert.h>
#include <stdio.h>

void test_defragmenter() {
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
    assert(stats.used_memory == 0);
    assert(stats.free_memory == 1024 * 1024 - sizeof(block_header_t));
    assert(stats.total_free == 2);

    printf("test_defragmenter passed.\n");
}

int main() {
    test_defragmenter();
    printf("All tests passed.\n");
    return 0;
}