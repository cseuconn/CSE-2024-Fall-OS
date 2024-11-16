#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

// a function that sorts the jobs by arrival time using bubble sort
void sort_arrival_time(struct thread_info* jobs_array, int numJobs)
{
    for ( int i = 0; i < numJobs - 1; i++)
    {
        for ( int j = 0; j < numJobs - i - 1; j++ )
        {
            if (jobs_array[j].arrival_time > jobs_array[j+1].arrival_time)
            {
                // swap the jobs
                struct thread_info temp = jobs_array[j];
                jobs_array[j] = jobs_array[j+1];
                jobs_array[j+1] = temp;
            }
        }
    }
}


void fifo_in_place(struct thread_info* jobs_array)
{
    // get the number of jobs in the array
    int numJobs = sizeof(jobs_array) / sizeof(jobs_array[0]);
    
    // sort the jobs based on arrival time using a bubble sort
    sort_arrival_time(jobs_array, numJobs);

    int curr_time = 0;

    // process the jobs through FIFO
    for ( int i = 0; i < numJobs; i++)
    {
        // get the job
        struct thread_info* job = &jobs_array[i];

        // wait for the job to arrive and go to that specific time
        if ( curr_time < job->arrival_time )
        {
            curr_time = job->arrival_time;
        }

        // set the completion time
        job->completion_time = curr_time + job->time_remaining;   

        // update the current time
        curr_time = job->completion_time;     
    }
}

void setup(struct thread_info *jobs, int * job_info)
{
    int j = 0;
    for (int i = 0; i < 10; i = i + 2)
    {
        jobs[j].job_ID = i;
        jobs[j].arrival_time = job_info[i];
        jobs[j].turnaround_time = -1;
        jobs[j].completion_time = -1;
        jobs[j].time_remaining = job_info[i+1];
        jobs[j].first_run = -1;
        jobs[j].response_time = -1;
        j++;
    }
}

int main(int argc, char *argv[])
{
    struct thread_info *job_list = malloc(5 * sizeof(struct thread_info));
    int job_expected_times_fifo[5][2] = {{5, 0}, {18, 5}, {18, 18}, {35, 21}, {30, 39}};
    int job_expected_times_sfj[5][2] = {{5, 0}, {18, 8}, {18, 18}, {36, 21}, {30, 39}};
    int job_starter_info[10] = {0, 5, 0, 13, 3, 3, 4, 18, 19, 10};
    
    setup(job_list, job_starter_info);
    
    printf("Running FIFO tests\n");
    fifo_in_place(job_list);

    for (int i = 0; i < 5; i++)
    {
        printf("Job %d expected turnaround time: %d, actual turnaround time: %d\n",
                i, job_expected_times_fifo[i][0], job_list[i].turnaround_time);
        printf("Job %d expected response time: %d, actual response time: %d\n\n",
                i, job_expected_times_fifo[i][1], job_list[i].response_time);
    }

    printf("Running SFJ tests\n");
    //sjf_function(job_list);

    for (int i = 0; i < 5; i++)
    {
        printf("Job %d expected turnaround time: %d, actual turnaround time: %d\n",
                i, job_expected_times_sfj[i][0], job_list[i].turnaround_time);
        printf("Job %d expected response time: %d, actual response time: %d\n\n",
                i, job_expected_times_sfj[i][1], job_list[i].response_time);
    }

    return 0;
}
