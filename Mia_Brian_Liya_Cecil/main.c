#include "thread.h"
#include "FIFO.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    struct thread_info job_list[5];
    int job_expected_times[5][2];
    int scheduler;

    printf('Running FIFO tests\n');
    fifo_in_place(&job_list);
    for (int i = 0; i < 5; i++)
    {
        printf('Job %d expected turnaround time: %d, actual turnaround time: %d',
                i, job_list[i].turnaround_time, job_expected_times[i][0]);
        printf('Job %d expected response time: %d, actual response time: %d',
                i, job_list[i].response_time, job_expected_times[i][1]);
    }
}
