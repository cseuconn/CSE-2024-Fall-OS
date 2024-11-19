# Memory Allocation Application API Reference

## Memory Allocator

### Initialization
```c
bool mem_init(size_t pool_size);
```
Initializes the memory allocator with specified pool size.
- **Parameters:** 
  - `pool_size`: Size of memory pool in bytes
- **Returns:** `true` if successful, `false` otherwise

### Memory Allocation
```c
void* mem_alloc(size_t size);
```
Allocates memory block of requested size.
- **Parameters:**
  - `size`: Requested size in bytes
- **Returns:** Pointer to allocated memory or NULL if failed

### Memory Deallocation
```c
void mem_free(void* ptr);
```
Frees allocated memory block.
- **Parameters:**
  - `ptr`: Pointer to memory block

### Statistics
```c
mem_stats_t mem_get_stats(void);
```
Returns current memory statistics.
- **Returns:** Structure containing memory usage statistics

### Cleanup
```c
void mem_cleanup(void);
```
Cleans up memory allocator and frees memory pool.

## Garbage Collector

### Initialization
```c
bool gc_init(void);
```
Initializes the garbage collector.
- **Returns:** `true` if successful, `false` otherwise

### Mark Operation
```c
void gc_mark(void* ptr);
```
Marks memory block as reachable.
- **Parameters:**
  - `ptr`: Pointer to memory block

### Collection
```c
void gc_collect(void);
```
Runs garbage collection cycle.

### Cleanup
```c
void gc_cleanup(void);
```
Cleans up garbage collector.

## Memory Defragmenter

### Initialization
```c
bool defragmenter_init(void);
```
Initializes the memory defragmenter.
- **Returns:** `true` if successful, `false` otherwise

### Defragmentation
```c
void defragmenter_run(void);
```
Runs memory defragmentation.

### Cleanup
```c
void defragmenter_cleanup(void);
```
Cleans up defragmenter.

## Data Structures

### Memory Statistics
```c
typedef struct {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;
    size_t total_allocation;
    size_t total_free;
    size_t fragments;
} mem_stats_t;
```

### Block Header
```c
typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
    struct block_header* prev;
    struct block_header* next_free;
} block_header_t;
```

## Error Handling

All functions that can fail return either NULL pointers or boolean status.
Check return values for error conditions.

## Thread Safety

Current implementation is not thread-safe. Use external synchronization if needed.

## Best Practices

1. Always check return values
2. Free allocated memory when no longer needed
3. Run garbage collection periodically
4. Defragment memory when fragmentation is high
5. Monitor memory statistics for optimal performance
