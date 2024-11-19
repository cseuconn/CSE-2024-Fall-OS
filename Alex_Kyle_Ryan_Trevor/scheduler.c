#include <stdio.h>
#include "process.h"
#include "scheduler.h"

void sort_by_arrival_time(struct process processes[], int n){
    // Sort the processes by arrival time using insertion sort
    for (int i = 1; i < n; i++) {
        struct process key = processes[i];
        int j = i - 1;
        
        // Move the elements that are greater then the key to one position ahead
        while ((j >= 0) && (processes[j].arrival_time > key.arrival_time)) {
            processes[j + 1] = processes[j];
            j--;
        }

        processes[j+1] = key;
    }
}

void FIFO_Scheduling(struct process processes[], int n){
    //Sort the processes by arrival time using insertion sort
    sort_by_arrival_time(processes, n);

    //Start FIFO processes
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        // Wait if the current time is less than the process's arrival time
        //Takes care of CPU idle time
        if (current_time < processes[i].arrival_time) {
            printf("CPU idle from %d to %d\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }
        
        // Calculate the process's waiting time
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        total_waiting_time += processes[i].waiting_time;
        
        // Calculate the current time after the current process completes
        printf("Process %d starts at time %d\n", processes[i].pid, current_time);
        current_time += processes[i].burst_time;
        processes[i].remaining_time = 0;
        processes[i].is_completed = 1;
        printf("Process %d ends at time %d\n", processes[i].pid, current_time);

        // Calculate the processes completion and turnaround time
        processes[i].completion_time = current_time;
        processes[i].turnaround_time = current_time - processes[i].arrival_time;

        processes[i].completion_time = current_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}
void SJF_Scheduling(struct process processes[], int n)
{
    sort_by_arrival_time(processes, n);
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; i++)
    {
        // Start at soonest possibility
        if (current_time < processes[i].arrival_time)
        {
            printf("CPU idle from %d to %d\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }
        
        // Find shortest process that has arrived and not completed
        int j = 0; // incrementer
        int k = 0; // next process to execute
        int sbt = __INT_MAX__;
        int done = 0;

        while (!done) {
            if (processes[j].arrival_time > current_time) done = 1;
            else if ((processes[k].is_completed) || ((!processes[j].is_completed) && (processes[j].burst_time < sbt))) {
                k = j;
                sbt = processes[k].burst_time;
            }
            j++;
            if (j >= n) done = 1;
        }

        // Calculate the process's waiting time
        processes[k].waiting_time = current_time - processes[k].arrival_time;
        total_waiting_time += processes[k].waiting_time;
        
        // Complete the task and caclulate time passed
        printf("Process %d starts at time %d\n", processes[k].pid, current_time);
        current_time += processes[k].burst_time;
        processes[k].remaining_time = 0;
        processes[k].is_completed = 1;
        printf("Process %d ends at time %d\n", processes[k].pid, current_time);

        // Calculate the process's completion and turnaround time
        processes[k].completion_time = current_time;
        processes[k].turnaround_time = current_time - processes[k].arrival_time;
        total_turnaround_time += processes[k].turnaround_time;
    }
    
    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}

void LJF_Scheduling(struct process processes[], int n)
{
    sort_by_arrival_time(processes, n);
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; i++)
    {
        // Start at soonest possibility
        if (current_time < processes[i].arrival_time)
        {
            printf("CPU idle from %d to %d\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }
        
        // Find shortest process that has arrived and not completed
        int j = 0; // incrementer
        int k = 0; // next process to execute
        int lbt = 0;
        int done = 0;

        while (!done) {
            if (processes[j].arrival_time > current_time) done = 1;
            else if ((processes[k].is_completed) || ((!processes[j].is_completed) && (processes[j].burst_time > lbt))) {
                k = j;
                lbt = processes[k].burst_time;
            }
            j++;
            if (j >= n) done = 1;
        }

        // Calculate the process's waiting time
        processes[k].waiting_time = current_time - processes[k].arrival_time;
        total_waiting_time += processes[k].waiting_time;
        
        // Complete the task and caclulate time passed
        printf("Process %d starts at time %d\n", processes[k].pid, current_time);
        current_time += processes[k].burst_time;
        processes[k].remaining_time = 0;
        processes[k].is_completed = 1;
        printf("Process %d ends at time %d\n", processes[k].pid, current_time);

        // Calculate the process's completion and turnaround time
        processes[k].completion_time = current_time;
        processes[k].turnaround_time = current_time - processes[k].arrival_time;
        total_turnaround_time += processes[k].turnaround_time;
    }
    
    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}

void RR_Scheduling(struct process processes[], int n, int time_quantum){
    // Sort the processes by arrival time using insertion sort
    sort_by_arrival_time(processes, n);

    // Start Round Robin process
    int current_time = 0;
    int done;

    do{
        // Start by assuming all processes are done
        done = 1;

        // Iterate through processes to check next round
        for (int i = 0; i < n; i++){
            if (processes[i].remaining_time > 0){
                // Not all processes are done
                done = 0;

                 // If the process hasn't arrived yet, we go back to the beginning
                if(processes[i].arrival_time > current_time){
                    // By resetting i, we go back to the beginning, so processes[0] will run during this quantum
                    i = 0;
                }

                // Execute process for the time quantum or remaining time
                if (processes[i].arrival_time <= current_time){
                    int time_to_execute = processes[i].remaining_time < time_quantum? processes[i].remaining_time:time_quantum;
                    // Print process exacuting and current time 
                    printf("Process %d executes for %d units from time %d\n", processes[i].pid, time_to_execute, current_time);

                    // Update remaining and current time
                    current_time += time_to_execute;
                    processes[i].remaining_time -= time_to_execute;
                    
                    if (processes[i].remaining_time == 0){
                        processes[i].completion_time = current_time;
                        printf("Process %d completes at time %d\n", processes[i].pid, current_time);
                    }
                }
            }
        }
        
    }while(!done);
    // Calculate turnaround and waiting time for each process
    double total_turnaround_time = 0, total_waiting_time = 0;
    for(int i = 0; i < n; i++){
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }
    
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", total_turnaround_time / n);
}

void Priority_Scheduling(struct process processes[], int n)
{
    sort_by_arrival_time(processes, n);
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    
    for (int i = 0; i < n; i++)
    {
        // Start at soonest possibility
        if (current_time < processes[i].arrival_time)
        {
            printf("CPU idle from %d to %d\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }
        
        // Find process with the highest priority that has arrived and not completed
        int j = 0; // incrementer
        int k = 0; // next process to be executed
        int hp = __INT_MAX__; // highest priority (actually the lowest)
        int done = 0;

        while (!done) {
            if (processes[j].arrival_time > current_time) done = 1;
            else if ((processes[k].is_completed) || ((!processes[j].is_completed) && (processes[j].priority < hp))) {
                k = j;
                hp = processes[k].priority;
            }
            j++;
            if (j >= n) done = 1;
        }

        // Calculate the process's waiting time
        processes[k].waiting_time = current_time - processes[k].arrival_time;
        total_waiting_time += processes[k].waiting_time;
        
        // Complete the task and caclulate time passed
        printf("Process %d starts at time %d\n", processes[k].pid, current_time);
        current_time += processes[k].burst_time;
        processes[k].remaining_time = 0;
        processes[k].is_completed = 1;
        printf("Process %d ends at time %d\n", processes[k].pid, current_time);

        // Calculate the process's completion and turnaround time
        processes[k].completion_time = current_time;
        processes[k].turnaround_time = current_time - processes[k].arrival_time;
        total_turnaround_time += processes[k].turnaround_time;
    }
    
    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}

void SRTCF_Scheduling(struct process processes[], int n)
{
    // Keep track of total waiting time and total turnaround time for later use
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    // Sort the processes by arrival time
    sort_by_arrival_time(processes, n);

    // Time starts at 0
    int current_time = 0;

    // No processes have been completed yet
    int completed = 0;

    // Run until all processes are done
    while(completed < n)
    {
        int run_index = -1;                         // index of the process that will run at this time
        int shortest_remaining_time = __INT_MAX__;  // shortest remaining time in processes[] (processes[run_index].remaining_time)

        // Find which process has the shortest remaining time at this point in time
        for (int i = 0; i < n; i++)
        {
            // If the process has arrived and it's not done
            if ((processes[i].arrival_time <= current_time) && (processes[i].is_completed == 0))
            {
                // Check if it has less remaining time than the current shortest
                if (processes[i].remaining_time < shortest_remaining_time)
                {
                    // We need to update the waiting time for the previous shortest before updating the shortest
                    if (run_index != -1)
                    {
                        processes[run_index].waiting_time += 1;
                        total_waiting_time += 1;
                    }

                    // Update the run_index and shortest_remaining_time
                    run_index = i;
                    shortest_remaining_time = processes[i].remaining_time;
                }
                // If not, then it's waiting, so increment the waiting time
                else
                {
                    processes[i].waiting_time += 1;
                    total_waiting_time += 1;
                }
            }
        }

        // If the run_index is still stuck at -1, then no processes are qualified to run at this time
        if (run_index == -1)
        {
            current_time += 1;
            printf("CPU idle from %d to %d\n", current_time - 1, current_time);
            continue;
        }

        // The job at processes[run_index] will run for this time unit, decrement remaining_time accordingly
        processes[run_index].remaining_time -= 1;

        // Move to the next time unit
        current_time += 1;

        // Check if the process is done
        if (processes[run_index].remaining_time == 0)
        {
            processes[run_index].completion_time = current_time;
            processes[run_index].turnaround_time = current_time - processes[run_index].arrival_time;
            processes[run_index].is_completed = 1;
            completed += 1;
            total_turnaround_time += processes[run_index].turnaround_time;
        }
    }

    // Calculate average waiting time and turnaround time
    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}

void LRTCF_Scheduling(struct process processes[], int n)
{
    // Keep track of total waiting time and total turnaround time for later use
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    // Sort the processes by arrival time
    sort_by_arrival_time(processes, n);

    // Time starts at 0
    int current_time = 0;

    // No processes have been completed yet
    int completed = 0;

    // Run until all processes are done
    while(completed < n)
    {
        int run_index = -1;                 // index of the process that will run at this time
        int longest_remaining_time = 0;     // longest remaining time in processes[] (processes[run_index].remaining_time)

        // Find which process has the longest remaining time at this point in time
        for (int i = 0; i < n; i++)
        {
            // If the process has arrived and it's not done
            if ((processes[i].arrival_time <= current_time) && (processes[i].is_completed == 0))
            {
                // Check if it has more remaining time than the current longest
                if (processes[i].remaining_time > longest_remaining_time)
                {
                    // We need to update the waiting time for the previous longest before updating the longest
                    if (run_index != -1)
                    {
                        processes[run_index].waiting_time += 1;
                        total_waiting_time += 1;
                    }

                    // Update the run_index and shortest_remaining_time
                    run_index = i;
                    longest_remaining_time = processes[i].remaining_time;
                }
                // If not, then it's waiting, so increment the waiting time
                else
                {
                    processes[i].waiting_time += 1;
                    total_waiting_time += 1;
                }
            }
        }

        // If the run_index is still stuck at -1, then no processes are qualified to run at this time
        if (run_index == -1)
        {
            current_time += 1;
            printf("CPU idle from %d to %d\n", current_time - 1, current_time);
            continue;
        }

        // The job at processes[run_index] will run for this time unit, decrement remaining_time accordingly
        processes[run_index].remaining_time -= 1;

        // Move to the next time unit
        current_time += 1;

        // Check if the process is done
        if (processes[run_index].remaining_time == 0)
        {
            processes[run_index].completion_time = current_time;
            processes[run_index].turnaround_time = current_time - processes[run_index].arrival_time;
            processes[run_index].is_completed = 1;
            completed += 1;
            total_turnaround_time += processes[run_index].turnaround_time;
        }
    }

    // Calculate average waiting time and turnaround time
    double average_waiting_time = (double)total_waiting_time / n;
    double average_turnaround_time = (double)total_turnaround_time / n;
    printf("All processes complete after %d\n", current_time);
    printf("Average waiting time: %.2f\n", average_waiting_time);
    printf("Average turnaround time: %.2f\n", average_turnaround_time);
}
