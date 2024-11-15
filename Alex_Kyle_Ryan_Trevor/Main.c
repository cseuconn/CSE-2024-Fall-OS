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