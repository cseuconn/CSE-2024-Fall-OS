#include "thread.h"
#include <iostream>

using namespace std;

bool MyThread::operator<(const MyThread& otherThread) const {
    // If the threads have the same priority, the one that arrived first gets priority
    if(priority == otherThread.priority) {
        return toa > otherThread.toa;
    }
    else {
        return priority > otherThread.priority;
    }
}

// Returns the thread's turn around time
int MyThread::getTurnAround() {
    return (toc == -1) ? 0 : toc - toa; // Ensure no negative value
}

// Returns the thread's response time
long long MyThread::getResponseTime() {
    return (responseTime >= toa) ? responseTime - toa : 0;  // Ensure no negative value
}

//returns thread priority
int MyThread::getPriority(){
    return priority;
}

//set current thread priority
void MyThread::setPriority(int newPriority){
    priority = newPriority;
}

//age implementation
void MyThread::age(){
    // Increase the threads waiting time
    waitTime++;

    // If a threads as been waiting for longer than 3 units of time, increase the thread's priority and set the waiting time to 0
    if (waitTime > 3){
        if (priority > 1) {
            priority--;
            waitTime = 0;
            cout << "Thread " << id << "'s priority has increased to " << priority << endl;
        }
    }
}

