# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Source files and executable name
SOURCES = main.cpp cpu.cpp thread.cpp
EXEC = scheduler

# Default target to build the program
all: $(EXEC)

# Link the object files to create the executable
$(EXEC): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXEC)

# Clean up generated files
clean:
	rm -f $(EXEC)

# Phony target for cleaning
.PHONY: all clean
