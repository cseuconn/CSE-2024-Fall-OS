#include <stdio.h>
#include "scheduler.h"
#include "process.h"

void Scheduler_Menu(struct process proc[], int n)
{
    printf("Choose a scheduler from the following list:\n");
    printf("1. First In First Out (FIFO)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Round Robin (RR)\n");
    printf("4. Priority Scheduling\n");
    printf("5. Multilevel Queue Scheduling\n");
    printf("6. Shortest Remaining Time First (SRTCF)\n");
    int scheduler_choice;
    scanf("%d", &scheduler_choice);

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
            printf("Enter the time quantum: ");
            int quantum;
            scanf("%d", &quantum);
            RR_Scheduling(proc, n, quantum);
            break;
        case 4:
            printf("\nPriority Scheduling Scheduler\n");
            Priority_Scheduling(proc, n);
            break;
        case 5:
            printf("\nMultilevel Queue Scheduling Scheduler\n");
            //MultilevelQueue_Scheduling(proc, n);
            break;
        case 6:
            printf("\nShortest Remaining Time First (SRTCF) Scheduler\n");
            SRTCF_Scheduling(proc, n);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}

void print_scheduler_menu(){
    printf("Choose a scheduler from the following list:\n");
    printf("1. First In First Out (FIFO)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Round Robin (RR)\n");
    printf("4. Priority Scheduling\n");
    printf("5. Multilevel Queue Scheduling\n");
    printf("6. Shortest Remaining Time First (SRTCF)\n");
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

    printf("Would you like to compare the performance of 2 or more schedulers? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    int num_schedulers;

    if (choice == 'y') {
        printf("Enter the number of schedulers you would like to compare: ");
        scanf("%d", &num_schedulers);
        while (num_schedulers < 2) {
            printf("Please enter a number greater than 1: ");
            scanf("%d", &num_schedulers);
        }
        printf("\n");

        int schedulers[num_schedulers]; 
        int quantum;
        print_scheduler_menu();
        for (int i = 0; i < num_schedulers; i++) {
            printf("Choose scheduler %d: ", i + 1);
            scanf("%d", &schedulers[i]);
            if (schedulers[i] == 3) {
                printf("Enter the time quantum for scheduler %d: ", i + 1);
                scanf("%d", &quantum);
            }
        }

        for (int i = 0; i < num_schedulers; i++) {
            switch (schedulers[i]) {
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
                    RR_Scheduling(proc, n, quantum);
                    break;
                case 4:
                    printf("\nPriority Scheduling Scheduler\n");
                    Priority_Scheduling(proc, n);
                    break;
                case 5:
                    printf("\nMultilevel Queue Scheduling Scheduler\n");
                    //MultilevelQueue_Scheduling(proc, n);
                    break;
                case 6:
                    printf("\nShortest Remaining Time First (SRTCF) Scheduler\n");
                    SRTCF_Scheduling(proc, n);
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
            }
        }
    }
    else {
        Scheduler_Menu(proc, n);
    }
    return 0;
}
