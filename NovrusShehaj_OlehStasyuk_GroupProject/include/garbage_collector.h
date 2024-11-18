#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <stddef.h>
#include <stdbool.h>

// Initialize Garbage Collector
bool gc_init(void);

// Mark a block as reachable
void gc_mark(void* ptr);

// Run the garbage collection algorithm
void gc_collect(void);

// Cleanup the Garbage Collector
void gc_cleanup(void);

#endif // GARBAGE_COLLECTOR_H