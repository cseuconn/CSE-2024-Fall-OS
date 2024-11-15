#include <stdio.h>
#include "Scheduler.h"

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int remaining_time;
    int is_completed;
};


void sort_by_arrival_time(struct process processes[], int n){
    //Sort the processes by arrival time using insertion sort
    for(int i = 1; i < n; i++){
        struct process key = processes[i];
        int j = i - 1;
        
        //Move the elements that are greater then the key to one position ahead
        while(j >=0 && processes[j].arrival_time > key.arrival_time){
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

    for(int i =0; i < n; i++){
        // Wait if the current time is less than the process's arrival time
        //Takes care of CPU idle time
        if(current_time < processes[i].arrival_time){
            printf("CPU idle from %d to %d\n", current_time, processes[i].arrival_time);
            current_time = processes[i].arrival_time;
        }

        //Calculate the current time after the current process completes
        printf("Process %d starts at time %d\n", processes[i].pid, current_time);
        current_time += processes[i].burst_time;
        printf("Process %d ends at time %d\n", processes[i].pid, current_time);

        //Calculate the processes completion, turnaround, and waiting time
        processes[i].completion_time = current_time;
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }

    //Display results after if we want to use a function instead of printing in the method like above
    //printf("After scheduling:\n");
    //....
}

void RR_Scheduling(struct process processes[], int n, int time_quantom){
    //Sort the processes by arrival time using insertion sort
    sort_by_arrival_time(processes, n);

    //Start Round Robin process
    int current_time = 0;
    int done;

    do{
        //Start by assuming all processes are done
        done = 1;

        //Iterate through processes to check next round
        for(int i = 0; i < n; i++){
            if(processes[i].remaining_time > 0){
                //Not all processes are done
                done = 0;

                //Handle CPU idle time
                if(processes[i].arrival_time > current_time){
                    //Find the earliest arrival time of remaining process
                    int earliest_arrival = processes[i].arrival_time;
                    for(int j = 0; j < n; j++){
                        if (processes[j].remaining_time > 0 && processes[j].arrival_time < earliest_arrival) {
                            earliest_arrival = processes[j].arrival_time;
                        }
                    }

                    // Move current time to the earliest available process
                    current_time = earliest_arrival;
                }

                //Exacute process for the time quantom or remaing time
                if(processes[i].arrival_time <= current_time){
                    int time_to_exacute = processes[i].remaining_time < time_quantom? processes[i].remaining_time:time_quantom;
                    //Print process exacuting and current time 
                    printf("Process %d executes for %d units from time %d\n", processes[i].pid, time_to_exacute, current_time);

                    //Update remainging and current time
                    current_time += time_to_exacute;
                    processes[i].remaining_time -= time_to_exacute;
                    
                    if(processes[i].remaining_time == 0){
                        processes[i].completion_time = current_time;
                        printf("Process %d completes at time %d\n", processes[i].pid, current_time);
                    }
                }
            }
        }

    }while(!done);

    //Calculate turnaround and waiting time for each process
    for(int i = 0; i < n; i++){
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }


    //Display results after if we want to use a function instead of printing in the method like above
   // printf("After Scheduling:\n"); ...
}

int main(int argc, char *argv[]) {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct process proc[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time of process %d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
        proc[i].pid = i + 1;
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].is_completed = 0;
    }
    printf("\n");

    printf("Would you like to compare the performance of 2 or more schedulars? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y') {
        int num_schedulers;
        printf("Enter the number of schedulars you would like to compare: ");
        scanf("%d", &num_schedulers);
        while (num_schedulers < 2) {
            printf("Please enter a number greater than 1: ");
            scanf("%d", &num_schedulers);
        }  
        printf("\n");

        for (int i = 0; i < num_schedulers; i++){
            printf("Choose the schedular from the following list:\n");
            printf("1. First Come First Serve (FCFS)\n");
            printf("2. Shortest Job First (SJF)\n");
            printf("3. Round Robin (RR)\n");
            printf("4. Priority Scheduling\n");
            printf("5. Multilevel Queue Scheduling\n");
            int schedular_choice;
            scanf("%d", &schedular_choice);

            void (*scheduler_functions[]) (struct process*, int) = {FCFS, SJF, RR, Priority, MultilevelQueue};

            switch (schedular_choice) {
                case 1:
                    printf("\nFirst Come First Serve (FCFS) Schedular\n");
                    FCFS(proc, n);
                    break;
                case 2:
                    printf("\nShortest Job First (SJF) Schedular\n");
                    SJF(proc, n);
                    break;
                case 3:
                    printf("\nRound Robin (RR) Schedular\n");
                    printf("Enter the time quantum: ");
                    int quantum;
                    scanf("%d", &quantum);
                    RR(proc, n, quantum);
                    break;

                case 4:
                    printf("\nPriority Scheduling Schedular\n");
                    Priority(proc, n);
                    break;
                case 5:
                    printf("\nMultilevel Queue Scheduling Schedular\n");
                    MultilevelQueue(proc, n);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
        }
    } 
    else {
        printf("Choose a schedular from the following list:\n");
        printf("1. First Come First Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF)\n");
        printf("3. Round Robin (RR)\n");
        printf("4. Priority Scheduling\n");
        printf("5. Multilevel Queue Scheduling\n");
        int schedular_choice;
        scanf("%d", &schedular_choice);

        switch (schedular_choice) {
            case 1:
                printf("\nFirst Come First Serve (FCFS) Schedular\n");
                FCFS(proc, n);
                break;
            case 2:
                printf("\nShortest Job First (SJF) Schedular\n");
                SJF(proc, n);
                break;
            case 3:
                printf("\nRound Robin (RR) Schedular\n");
                printf("Enter the time quantum: ");
                int quantum;
                scanf("%d", &quantum);
                RR(proc, n, quantum);
                break;
                
            case 4:
                printf("\nPriority Scheduling Schedular\n");
                Priority(proc, n);
                break;
            case 5:
                printf("\nMultilevel Queue Scheduling Schedular\n");
                MultilevelQueue(proc, n);
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }

    return 0;
}
