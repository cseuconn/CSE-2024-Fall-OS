cmake_minimum_required(VERSION 3.10)
project(memory_management_project)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/memory_allocator.c
    src/garbage_collector.c
    src/defragmenter.c
    src/root_pointers.c
)

set(CMAKE_BUILD_TYPE Debug)

# Add the executable
add_executable(memory_allocator
    src/main.c
    ${SOURCES}
)

# Add tests
enable_testing()

# Add root_pointers.c to the test_garbage_collector target
add_executable(test_garbage_collector
    tests/test_garbage_collector.c
    src/garbage_collector.c
    src/memory_allocator.c
    src/root_pointers.c
)

# Add root_pointers.c to the test_memory_allocator target if needed
add_executable(test_memory_allocator
    tests/test_memory_allocator.c
    src/memory_allocator.c
    src/root_pointers.c
)

# Add the necessary source files to the test_defragmenter target
add_executable(test_defragmenter
    tests/test_defragmenter.c
    src/defragmenter.c
    src/memory_allocator.c
    src/root_pointers.c
)

add_test(NAME TestMemoryAllocator COMMAND test_memory_allocator)
add_test(NAME TestGarbageCollector COMMAND test_garbage_collector)
add_test(NAME TestDefragmenter COMMAND test_defragmenter)