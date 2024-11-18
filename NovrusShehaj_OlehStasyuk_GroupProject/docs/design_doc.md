# Memory Allocation Application Design Document

## System Architecture

### Overview
The memory management system consists of three main components:
1. Memory Allocator
2. Garbage Collector 
3. Memory Defragmenter

### Components

#### 1. Memory Allocator
- Implements custom heap management with free list
- Uses block headers to track memory blocks
- Supports block splitting and coalescing
- Maintains memory usage statistics

**Key Data Structures:**
```c
typedef struct block_header {
    size_t size;
    bool is_free;
    struct block_header* next;
    struct block_header* prev;
    struct block_header* next_free;
} block_header_t;

typedef struct {
    block_header_t* first_block;
    block_header_t* free_list;
    mem_stats_t stats;
    void* memory_pool;
    size_t pool_size;
} memory_allocator_t;
```

#### 2. Garbage Collector
- Implements mark-and-sweep algorithm
- Tracks root pointers
- Marks reachable blocks
- Sweeps unreachable blocks
    - Algorithm:
        1. Mark Phase: Traverse from root pointers
        2. Sweep Phase: Free unmarked blocks
        3. Update statistics

#### 3. Memory Defragmenter
- Compacts memory to reduce fragmentation
- Uses sliding window algorithm
- Maintains block continuity
- Updates pointers after moving blocks

### Memory Block Layout
+----------------+----------------+
| Block Header   | User Data     |
+----------------+----------------+
| size          |                |
| is_free       |                |
| next/prev     |                |
| next_free     |                |
+----------------+----------------+

### Algorithms

#### Memory Allocation
1. Search free list for suitable block
2. Split block if too large
3. Remove from free list
4. Update statistics

#### Block Splitting
1. Check minimum block size
2. Create new header
3. Update pointers
A4. dd to free list

#### Block Coalescing
Check adjacent blocks
Merge if free
Update free list
Update statistics

### Performance Considerations

#### Time Complexity
- Allocation: O(n) worst case
- Deallocation: O(1)
- arbage Collection: O(n)
- Defragmentation: O(n)

#### Space Overhead
- Block Header: 32 bytes
- Minimum Block Size: 32 bytes
- Alignment: 8 bytes

#### Error Handling
- NULL return for failed allocations
- Assertion checks in debug builds
- Statistics tracking for diagnostics

#### Future Improvements
1. Generational garbage collection
2. Thread safety
3. Memory pooling
