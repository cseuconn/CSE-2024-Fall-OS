# CSE-2024-Fall-05
## Simple Operating System Implementation
This project implements basic operating system components including process management, memory management, synchronization primitives (mutexes and semaphores), and a basic filesystem. It's designed as an educational tool to demonstrate core OS concepts.
### Environment Requirements

Operating System: Windows
Compiler: GCC (MinGW-w64)
Build System: GNU Make

### Prerequisites
1. Install MinGW-w64:

Download the installer from MinGW-w64
During installation, select: 
Architecture: x86_64
Threads: posix
Exception: seh

Add MinGW-w64 bin directory to your system PATH

2. Verify installation:
make --version

### Required Packages
pthread (included with MinGW-w64)
Standard C libraries

### Building and Running Tests
- make clean
- make all
- make test

This will build and run tests for:

Process Management
Memory Management
Filesystem Operations
Synchronization Implementation
