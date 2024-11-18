#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "thread.h"
#include <queue>
#include <vector>

class Scheduler {
public:
    Scheduler(int timeSlice);
    
    // Adds a thread to the ready queue
    void addThread(const MyThread &thread);

    // Runs the highest-priority thread and manages preemption if needed
    void runNextThread(MyThread &runningThread, int currentTime);

    // Checks if a thread in the ready queue should preempt the running thread
    bool shouldPreempt(const MyThread &runningThread) const;

    // Returns true if the ready queue is empty
    bool isReadyQueueEmpty() const;

private:
    std::priority_queue<MyThread> readyQueue;
    int timeSlice;
};

#endif // SCHEDULER_H
