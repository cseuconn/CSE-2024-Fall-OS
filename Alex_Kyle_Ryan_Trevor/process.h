#ifndef PROCESS_H
#define PROCESS_H

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int remaining_time;
    int is_completed;
    int priority;
};

#endif // PROCESS_H
