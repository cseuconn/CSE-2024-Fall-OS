#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

// calculate response time
void responsetime(struct thread_info *jobs, int n){
  for (int i = 0; i < n; i++){
    jobs[i].response_time = jobs[i].first_run - jobs[i].arrival_time;
  }
}

// calculate turnaround time
void turnaroundtime(struct thread_info *jobs, int n){
  for(int i = 0; i < n; i++){
    jobs[i].turnaround_time = jobs[i].completion_time - jobs[i].arrival_time;
  }
}

// calculate average response time
double avg_responsetime(struct thread_info *jobs, int n){
  double total_responsetime = 0;
  for (int i = 0; i < n; i++){
    total_responsetime += jobs[i].response_time;
  }
  return total_responsetime / n;
}

// calculate average turnaround time 
double avg_turnaroundtime(struct thread_info *jobs, int n){
  double total_turnaroundtime = 0;
  for (int i = 0; i < n; i++){
    total_turnaroundtime += jobs[i].turnaround_time;
  }
  return total_turnaroundtime / n;
}

// print table of results for all jobs 
void print_results_table(struct thread_info *jobs, int n, int type){
  printf("Job ID\tArrival Time\tCompletion Time\tTurnaround Time\tResponse Time");
  for (int i = 0; i < n; i++){
    printf("%d\t%d\t%d\t%d\t%d\n", jobs[i].job_ID, jobs[i].arrival_time, jobs[i].completion_time, jobs[i].turnaround_time, jobs[i].response_time);
  }
  printf("Average Response Time: %d", avg_responsetime(jobs, n));
  printf("Average Turnaround Time: %d", avg_turnaroundtime(jobs, n));
}
