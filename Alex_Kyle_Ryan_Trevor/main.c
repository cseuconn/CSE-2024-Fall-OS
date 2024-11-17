#include <stdio.h>
#include "scheduler.h"
#include "process.h"

void print_scheduler_menu(){
    printf("Choose a scheduler from the following list:\n");
    printf("1. First In First Out (FIFO)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Round Robin (RR)\n");
    printf("4. Priority Scheduling\n");
    printf("5. Shortest Remaining Time First (SRTCF)\n");
}

void execute_scheduler(struct process proc[], int n, int scheduler_choice, int quantum){
    switch (scheduler_choice)
    {
        case 1:
            printf("\nFirst In First Out (FIFO) Scheduler\n");
            FIFO_Scheduling(proc, n);
            break;
        case 2:
            printf("\nShortest Job First (SJF) Scheduler\n");
            SJF_Scheduling(proc, n);
            break;
        case 3:
            printf("\nRound Robin (RR) Scheduler\n");
            if (quantum == 0) {
                printf("Enter the time quantum: ");
                scanf("%d", &quantum);
            }
            RR_Scheduling(proc, n, quantum);
            break;
        case 4:
            printf("\nPriority Scheduling Scheduler\n");
            Priority_Scheduling(proc, n);
            break;
        case 5:
            printf("\nShortest Remaining Time First (SRTCF) Scheduler\n");
            SRTCF_Scheduling(proc, n);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}

void single_choice_menu(struct process proc[], int n)
{
    print_scheduler_menu();
    int scheduler_choice;
    int quantum = 0;
    scanf("%d", &scheduler_choice);
    if (scheduler_choice == 3) {
        printf("Enter the time quantum: ");
        scanf("%d", &quantum);
    }
    if (scheduler_choice == 4) {
        for (int i = 0; i < n; i++) {
            printf("Enter priority of process %d: ", i + 1);
            scanf("%d", &proc[i].priority);
        }
    }
    execute_scheduler(proc, n, scheduler_choice, quantum);
}

void multiple_choice_menu(struct process proc[], int n){
    int num_schedulers;
    printf("Enter the number of schedulers you would like to compare: ");
    scanf("%d", &num_schedulers);
    while (num_schedulers < 2) {
        printf("Please enter a number greater than 1: ");
        scanf("%d", &num_schedulers);
    }
    printf("\n");

    int schedulers[num_schedulers];
    int quantum_nums[num_schedulers];

    print_scheduler_menu();
    for (int i = 0; i < num_schedulers; i++) {
        printf("Choose scheduler %d: ", i + 1);
        scanf("%d", &schedulers[i]);
        if (schedulers[i] == 3) {
            printf("Enter the time quantum for scheduler %d: ", i + 1);
            scanf("%d", &quantum_nums[i]);
        }
        else quantum_nums[i] = 0;
        if ((schedulers[i] == 4) && (proc[0].priority == -1)) {
            for (int i = 0; i < n; i++) {
                printf("Enter priority of process %d: ", i + 1);
                scanf("%d", &proc[i].priority);
            }
        }
    }

    for (int i = 0; i < num_schedulers; i++) {
        execute_scheduler(proc, n, schedulers[i], quantum_nums[i]);
        for (int p = 0; p < n; p++)
        {
            proc[p].completion_time = 0;
            proc[p].turnaround_time = 0;
            proc[p].waiting_time = 0;
            proc[p].is_completed = 0;
            proc[p].remaining_time = proc[p].burst_time;
        }
    }
}

void setup_procs(struct process proc[], int n){
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time of process %d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
        proc[i].pid = i + 1;
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].is_completed = 0;
        proc[i].priority = -1;
    }
}

int main(int argc, char *argv[]) {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct process proc[n];
    setup_procs(proc, n);
    printf("\n");

    printf("Would you like to compare the performance of 2 or more schedulers? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y') multiple_choice_menu(proc, n);
    else single_choice_menu(proc, n);
    
    return 0;
}