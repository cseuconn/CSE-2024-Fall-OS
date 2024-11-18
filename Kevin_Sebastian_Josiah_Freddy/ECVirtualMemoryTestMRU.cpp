#include "ECVirtualMemoryMRU.h"
#include <algorithm>
using namespace std;

// Test utility
template<class T>
void ASSERT_EQ(T x, T y)
{
  if( x == y )
  {
    cout << "Test passed: equal: " << x << "  " << y << endl;
  }
  else
  {
    cout << "Test FAILED: equal: " << x << "  " << y << endl;
  }
}

// Run MRU algorithm on a list of page requests
static void RunMRU(int *listPageRequests, int numPageRequests, int memoryCapacity, int numPageFaultsExpected)
{
    // Create virtual memory
    ECVirtualMemory mem(memoryCapacity);

    // Now run MRU
    for(int i=0; i<numPageRequests; ++i)
    {
      // access the page
      mem.AccessPage(listPageRequests[i]);
    }
    //cout << "Number of page faults: " << numPageFaults << endl;
    // Replacement miss at the first four pages plus 5, 1, 6 2 page access
    int numPageFaults = mem.GetNumPageFaults();
    ASSERT_EQ( numPageFaults, numPageFaultsExpected);
}

// Run optimal algorihtm
static void RunOpt(int *listPageRequests, int numPageRequests, int memoryCapacity, int numPageFaultsExpected)
{
    // Create virtual memory
    ECVirtualMemory mem(memoryCapacity);

    // Now run MRU
    for(int i=0; i<numPageRequests; ++i)
    {
      // access the page
      mem.AccessPage(listPageRequests[i]);
    }

    // now get opt result
    int numPageFaults = mem.RunOpt();

    // Replacement miss at the first four pages plus 5, 6 page access
    ASSERT_EQ( numPageFaults, numPageFaultsExpected);
}

// Test case 1

static void Test() {
    cout << "******** Test1...\n";
    int pageList[20] = {1, 2, 3, 4, 5, 1, 3, 1, 6, 3, 2, 3};
    RunMRU( pageList, 12, 4, 6  );
    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 12, 4, 6);
}

// Test case 2
static void Test2() {
    cout << "******** Test2...\n";
    int pageList[10] = {4, 7, 6, 1, 7, 6, 1, 2, 7, 2};
    RunMRU( pageList, 10, 3, 8  );
    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 10, 3, 5);
}

static void Test3() {
    cout << "******** Test3...\n";
    int pageList[22] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1};
    RunMRU( pageList, 22, 3, 18 );
    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 22, 3, 9);
}

// Test case 4
static void Test4()
{
    cout << "******** Test4...\n";
    //
    int pageList[12] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    RunMRU( pageList, 12, 3, 7 );

    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 12, 3, 7);
}

// Test case 5
static void Test5()
{
    cout << "******** Test5...\n";
    //
    int pageList[10] = {3, 1, 4, 2, 5, 2, 1, 2, 3, 4};
    RunMRU( pageList, 10, 4, 6 );

    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 10, 4, 6);
}

// Test case 6
static void Test6()
{
    cout << "******** Test6...\n";
    //
    int pageList[20] = {1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6};

    // now small memory: 3 pages
    RunMRU( pageList, 20, 3, 16 );

    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 20, 3, 11);
    
    // now large memory: 5 pages
    RunMRU( pageList, 20, 5, 9 );

    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 20, 5, 7);
    
    // now use larger memory: 7 pages
    RunMRU( pageList, 20, 7, 7 );

    // find the optimal missing
    cout << "Now run the optimal algorithm...\n";
    RunOpt(pageList, 20, 7, 7);
}

int main()
{
    try
    {
        Test();
        Test2();
        Test3();
        Test4();
        Test5();
        Test6();
    }
    catch(string ex)
    {
        cout << "Exception: " << ex << endl;
    }
}

