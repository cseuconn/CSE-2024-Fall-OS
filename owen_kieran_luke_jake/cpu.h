#ifndef CPU_HEADER
#define CPU_HEADER

#include "thread.h"
#include <queue>
#include <string>
#include <vector>

using namespace std;

class MyCpu {
public:
    // Creates MyCpu class with time = 0 the default
    MyCpu();

    // Load files from a file (currently disabled)
    int loadThreadsFromFile(string filename);

    // Load one thread
    int loadThread(MyThread thread);

    // Function to run the next thread in the readyQueue
    int runNextThread();

    // Run the CPU which wont stop until all the threads are done
    int runCPU();

    // print all the threads in the readyQueue (was used mostly for testing/debugging)
    // *IMPORTANT* As of now, it will remove all the threads from the queue as it prints them
    void printReadyThreads();

    // print all threads once completed
    void printCompletedThreads();

    // print cpu stats (average response time, average turnaround)
    void printCPUStats();
  
    // print threads arrival time
    void printArrivalTimes();

    // Set the time that the CPU is at
    void setTime(int newTime) {time = newTime;}

    // Set how much time the CPU works on one thread for
    void setTimeSlice(int newTimeSlice) {timeSlice = newTimeSlice;}

    // Function to age ready threads
    void ageThreads();

private:
    // Threads that will arrive in the future
    vector<MyThread> futureThreads;

    // All threads that are ready
    priority_queue<MyThread> readyQueue;

    // Completed threads
    vector<MyThread> completedThreads;

    // The one thread that is running
    MyThread running;

    // Time interval and how much time CPU work on one thread
    int time;
    int timeSlice;
};

#endif