#ifndef SCHEDULER_H  
#define SCHEDULER_H

#include "process.h"

void sort_by_arrival_time(struct process processes[], int n);
void FIFO_Scheduling(struct process processes[], int n);
void SJF_Scheduling(struct process processes[], int n);
void LJF_Scheduling(struct process processes[], int n);
void SRTCF_Scheduling(struct process processes[], int n);
void LRTCF_Scheduling(struct process processes[], int n);
void RR_Scheduling(struct process processes[], int n, int time_quantum);
void Priority_Scheduling(struct process processes[], int n);

#endif
