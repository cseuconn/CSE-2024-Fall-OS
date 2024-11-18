#include "ECVirtualMemory.h"
#include <algorithm>
using namespace std;

// Simulate RAND page replacement algorithm on a list of page requests
static void RunRAND(int *ListPageRequests, int numPageRequests, int memoryCapacity) {
    // Create virtual memory with RAND algorithm
    ECVirtualMemory mem(memoryCapacity);

    // RAND Algorithm: Simulate the page access for each page request
    for (int i = 0; i < numPageRequests; ++i) {
        int page = ListPageRequests[i];
        mem.AccessPage(page);
    }

    // Get the number of page faults from the memory management system
    int numPageFaults = mem.GetNumPageFaults();

    // Output the result for manual verification due to randomness
    cout << "Total page faults: " << numPageFaults << endl;
}

static void Test1() {
    cout << "******** Test1: RAND Algorithm ********\n";
    int pageList[12] = {1, 2, 3, 4, 5, 1, 3, 1, 6, 3, 2, 3};
    RunRAND(pageList, 12, 4); // Memory capacity is 4
}

static void Test2() {
    cout << "******** Test2: RAND Algorithm ********\n";
    int pageList[12] = {4, 7, 6, 1, 7, 6, 1, 2, 7, 2};
    RunRAND(pageList, 10, 3); // Memory capacity is 3
}

static void Test3() {
    cout << "******** Test3: RAND Algorithm ********\n";
    int pageList[12] = {0, 1, 2, 3, 0, 4, 1, 2, 3, 0, 4};
    RunRAND(pageList, 11, 3); // Memory capacity is 3
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}