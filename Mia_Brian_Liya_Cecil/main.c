#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

typedef struct {
    int job_ID;
    int arrival_time;
    int turnaround_time;
    int completion_time;
    int time_remaining;
    int first_run;  
    int response_time;
} job_info;

int main(int argc, char *argv[])
{
    job_info job_list[5];
}