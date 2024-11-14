cmake_minimum_required(VERSION 3.10)
project(memory_management_project)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.c
    src/memory_allocator.c
    src/garbage_collector.c
    src/defragmenter.c
)

# Add the executable
add_executable(memory_management_app ${SOURCES})

# Add tests
enable_testing()
add_executable(test_memory_allocator tests/test_memory_allocator.c src/memory_allocator.c)
add_executable(test_garbage_collector tests/test_garbage_collector.c src/garbage_collector.c src/memory_allocator.c)
add_executable(test_defragmenter tests/test_defragmenter.c src/defragmenter.c src/memory_allocator.c)

add_test(NAME TestMemoryAllocator COMMAND test_memory_allocator)
add_test(NAME TestGarbageCollector COMMAND test_garbage_collector)
add_test(NAME TestDefragmenter COMMAND test_defragmenter)