#include "thread.h"
#include <stdio.h>
#include <stdlib.h>



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

         // set the completion time and first run time
        job->first_run = curr_time;
        job->completion_time = curr_time + job->time_remaining;   
        
        // update the current time
        curr_time = job->completion_time;     
    }
}




struct thread_info* fifo_not_in_place(struct thread_info* jobs_array)
{
    // get the number of jobs in the array and intialize a results array
    int numJobs = sizeof(jobs_array) / sizeof(jobs_array[0]);
    struct thread_info* results = (struct thread_info*)malloc(numJobs * sizeof(struct thread_info));

    int curr_time = 0;

    // process the jobs through FIFO
    for ( int i = 0; i < numJobs; i++)
    {
        // copy over the jobs from the passed in array into the new one
        results[i] = jobs_array[i];
        
        // get the job
        struct thread_info* job = &results[i];

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
    
    return results;
}

