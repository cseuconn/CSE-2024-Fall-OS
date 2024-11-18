# CSE-2024-Fall-OS

# Group Project - Memory Allocation Application

# Team Members: Novrus Shehaj and Oleh Stasyuk

# Advanced Memory Management Project

## Overview

This project implements an advanced memory management system in C, including dynamic memory allocation, garbage collection, and memory defragmentation. The system is designed to efficiently manage memory usage and optimize performance through custom algorithms and data structures.

## Project Structure

The project is organized into the following directories and files:

memory-management-project/
├── CMakeLists.txt
├── README.md
├── design_doc.md
├── src/
│   ├── main.c
│   ├── memory_allocator.c
│   ├── memory_allocator.h
│   ├── garbage_collector.c
│   ├── garbage_collector.h
│   ├── defragmenter.c
│   ├── defragmenter.h
├── tests/
│   ├── test_memory_allocator.c
│   ├── test_garbage_collector.c
│   ├── test_defragmenter.c
├── include/
│   ├── memory_allocator.h
│   ├── garbage_collector.h
│   ├── defragmenter.h
├── docs/
│   ├── design_doc.md
│   ├── api_reference.md
└── .github/
    ├── workflows/
        ├── ci.yml


### Directory and File Descriptions

- **CMakeLists.txt**: Configuration file for building the project using CMake.
- **README.md**: Project overview, setup instructions, and usage examples.
- **design_doc.md**: Detailed design document outlining the architecture, algorithms, and data structures used.
- **src/**: Source code directory.
  - **main.c**: Entry point of the application.
  - **memory_allocator.c**: Implementation of the memory allocation functions.
  - **memory_allocator.h**: Header file for memory allocation functions.
  - **garbage_collector.c**: Implementation of the garbage collection functions.
  - **garbage_collector.h**: Header file for garbage collection functions.
  - **defragmenter.c**: Implementation of the memory defragmentation functions.
  - **defragmenter.h**: Header file for memory defragmentation functions.

- **tests/**: Unit tests for the project.
  - **test_memory_allocator.c**: Tests for memory allocation functions.
  - **test_garbage_collector.c**: Tests for garbage collection functions.
  - **test_defragmenter.c**: Tests for memory defragmentation functions.

- **include/**: Header files for the project.
  - **memory_allocator.h**: Header file for memory allocation functions.
  - **garbage_collector.h**: Header file for garbage collection functions.
  - **defragmenter.h**: Header file for memory defragmentation functions.

- **docs/**: Documentation for the project.
  - **design_doc.md**: Detailed design document.
  - **api_reference.md**: API reference for the project.
- **.github/**: GitHub Actions workflows.
  - **ci.yml**: Continuous integration configuration.

## Setup

### Prerequisites

- CMake 3.10 or higher
- GCC or Clang compiler

### Clone the Repository

```sh
git clone https://github.com/cseuconn/CSE-2024-Fall-OS.git
cd NovrusShehaj_OlehStasyuk_GroupProject
```

### Build the Project
1. Remove Current build directory if there is one and create a new build directory:
```sh
rm -rf build
mkdir build
cd build
```

2. Run CMake to generate build files:
```sh
cmake ..
```

3. Build the project:
```sh
make
```

## Run The Project

After building the project, you can run the application:
```sh
./memory_alloocator
```

## Run Tests
To run the unit tests, use the following command:
```sh
make test
```

## Usage
The application demonstrates the usage of the memory allocator, garbage collector, and defragmenter. Here is a brief overview of the main components:

### Memory Allocator
The memory allocator handles dynamic memory allocation and deallocation. It uses a custom heap management algorithm to efficiently allocate memory blocks.

#### Free List Management

The memory allocator uses a free list to manage free memory blocks. This allows for efficient allocation and deallocation of memory. The free list is a linked list of free blocks, and the allocator uses this list to find suitable blocks for allocation requests.

- **add_to_free_list**: Adds a block to the free list.
- **remove_from_free_list**: Removes a block from the free list.
- **split_block**: Splits a larger block into two smaller blocks if the remaining space is large enough to hold another block.
- **coalesce**: Merges adjacent free blocks into a single larger block to reduce fragmentation.

### Garbage Collector
The garbage collector automatically reclaims memory that is no longer in use. It uses a mark-and-sweep algorithm to identify and collect garbage.

### Memory Defragmenter
The memory defragmenter compacts memory to reduce fragmentation and improve allocation efficiency. It uses a sliding window algorithm to move memory blocks and consolidate free space.

## Example
Here is an example of how to use the memory management system in your application:
```c
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
```

## License
This project is licensed under the MIT License