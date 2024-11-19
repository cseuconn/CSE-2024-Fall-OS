#ifndef DEFRAGMENTER_H
#define DEFRAGMENTER_H

#include <stddef.h>
#include <stdbool.h>

// Initialize Defragmenter
bool defragmenter_init(void);

// Run the defragmentation algorithm
void defragmenter_run(void);

// Cleanup the Defragmenter
void defragmenter_cleanup(void);

#endif // DEFRAGMENTER_H