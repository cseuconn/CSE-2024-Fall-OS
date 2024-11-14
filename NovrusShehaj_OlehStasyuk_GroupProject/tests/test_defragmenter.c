#include "memory_allocator.h"
#include "defragmenter.h"
#include <assert.h>
#include <stdio.h>

void test_defragmenter() {
    // Initialize memory allocator and defragmenter
    assert(mem_init(1024 * 1024) == true);
    assert(defragmenter_init() == true);

    // Allocate memory
    void* ptr1 = mem_alloc(100);
    assert(ptr1 != NULL);

    void* ptr2 = mem_alloc(200);
    assert(ptr2 != NULL);

    // Free some memory to create fragmentation
    mem_free(ptr1);

    // Run defragmentation
    defragmenter_run();

    // Check memory statistics
    mem_stats_t stats = mem_get_stats();
    assert(stats.fragments == 0);

    // Cleanup
    mem_free(ptr2);
    defragmenter_cleanup();
    mem_cleanup();
    printf("Defragmenter Tests Passed\n");
}

int main() {
    test_defragmenter();
    return 0;
}