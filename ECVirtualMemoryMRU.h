#ifndef ECVirtualMemory_h
#define ECVirtualMemory_h

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>
using namespace std;

//*****************************************************************************
// Virtual memory: consists of memory pages and a main memory with limited capacity
// Page: represented by an integer; main memory can hold up to K pages
// Page replacement: when the main memory reaches its limit (i.e., having K pages) 
// and a new page (not currently in memory) is to be add, 
// then need to swap out one current page to make room for this new page
// This class: use MRU algorithm

class ECVirtualMemory
{
public:
    // Capacity: max # of pages in main memory 
    ECVirtualMemory(int capacity);

    // Access a page in memory
    void AccessPage(int page);

    int RunOpt();

    // Return the number of pages in main memory
    int GetNumPagesInMainMemory() const;

    // Return the number of page faults so far (for MRU algorithm)
    int GetNumPageFaults() const;

private:
    // Implementation utilities
    set<int> page_set;
    vector<int> page_list;
    vector<int> page_tracker;
    map<int, int> page_indices;
    int max_num_pages;
    int faults;
    int index;
    int prev_page;

};


#endif /* ECVirtualMemory_h */
