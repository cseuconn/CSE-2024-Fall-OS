#include "memory_allocator.h"
#include "garbage_collector.h"
#include "defragmenter.h"
#include <stdio.h>

int main() {
    // Initialize memory allocator with 1MB pool
    if (!mem_init(1024 * 1024)) {
        fprintf(stderr, "Failed to initialize memory allocator\n");
        return 1;
    }

    // Initialize garbage collector
    if (!gc_init()) {
        fprintf(stderr, "Failed to initialize garbage collector\n");
        mem_cleanup();
        return 1;
    }

    // Initialize defragmenter
    if (!defragmenter_init()) {
        fprintf(stderr, "Failed to initialize defragmenter\n");
        gc_cleanup();
        mem_cleanup();
        return 1;
    }

    // Allocate some memory
    void* ptr1 = mem_alloc(100);
    void* ptr2 = mem_alloc(200);

    // Mark ptr1 as reachable
    gc_mark(ptr1);

    // Run garbage collection
    gc_collect();

    // Run defragmentation
    defragmenter_run();

    // Print memory statistics
    mem_dump_stats();

    // Cleanup
    mem_free(ptr1);
    mem_free(ptr2);
    defragmenter_cleanup();
    gc_cleanup();
    mem_cleanup();

    return 0;
}