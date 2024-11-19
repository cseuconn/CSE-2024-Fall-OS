#ifndef THREAD_HEADER
#define THREAD_HEADER

class MyThread {
public:    
    // Thread constructors
    MyThread() {}
    MyThread(int id, int priority, long long toa, long long ttc): 
        id(id), priority(priority), waitTime(0), toa(toa), ttc(ttc), state(0), toc(-1), responseTime(-1) {}

    // operator overloading used for CPU's priority queue
    // Based off priority and toa
    bool operator<(const MyThread& otherThread) const;

    // Returns the thread's turn around time
    int getTurnAround();

    // Returns the thread's response time
    long long getResponseTime();

    //setter and getter for priority
    int getPriority();
    void setPriority(int newPriority);

    //age the thread
    void age();

    // id and priority of the thread
    int id;
    int priority;

    //time in ready or blocked state
    int waitTime;

    // Time of arrival
    long long toa;    

    // Time to complete
    long long ttc;    

    // 1: running, 2: ready, 3: blocked, 0: otherwise
    int state;

    // Time of completion
    long long toc;

    // These variables will be changed by the CPU to help calculate turn around, waiting time, and response time
    int turnAround;
    long long responseTime;
};

#endif