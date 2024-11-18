#include "scheduler.h"
#include <iostream>

// Constructor to set time slice
Scheduler::Scheduler(int timeSlice) : timeSlice(timeSlice) {}

// Adds a thread to the ready queue, considering its priority
void Scheduler::addThread(const MyThread &thread) {
    readyQueue.push(thread);
    std::cout << "Thread " << thread.id << " added to ready queue with priority " << thread.priority << "\n";
}

// Determines if the currently running thread should be preempted by a higher-priority thread
bool Scheduler::shouldPreempt(const MyThread &runningThread) const {
    if (readyQueue.empty()) return false;
    const MyThread &topThread = readyQueue.top();
    return topThread.priority < runningThread.priority;
}

// Runs the highest-priority thread from the ready queue, handling preemption if necessary
void Scheduler::runNextThread(MyThread &runningThread, int currentTime) {
    if (readyQueue.empty()) return;

    MyThread nextThread = readyQueue.top();
    readyQueue.pop();

    // If a higher-priority thread is found, preempt the current running thread
    if (shouldPreempt(runningThread)) {
        std::cout << "Preempting Thread " << runningThread.id << " for Thread " << nextThread.id << "\n";
        readyQueue.push(runningThread);  // Re-add current running thread to the ready queue
        runningThread = nextThread;      // Set the new thread as the running thread
    } else if (runningThread.id == -1) { // No thread is running currently
        runningThread = nextThread;
    } else {
        // Running thread continues if no preemption
        readyQueue.push(nextThread);
    }

    // Execute the running thread
    runningThread.ttc -= timeSlice;
    if (runningThread.ttc <= 0) {
        runningThread.turnAround = currentTime - runningThread.toa; // Calculate turnaround
        std::cout << "Thread " << runningThread.id << " completed.\n";
        runningThread.id = -1; // Reset running state
    }
}

// Checks if the ready queue is empty
bool Scheduler::isReadyQueueEmpty() const {
    return readyQueue.empty();
}
