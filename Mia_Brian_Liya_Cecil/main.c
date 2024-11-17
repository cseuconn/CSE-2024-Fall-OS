#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

struct thread_info* fifo_not_in_place(struct thread_info* jobs_array)
{
    // get the number of jobs in the array and intialize a results array
    int numJobs = 5;
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

        // set the start and completion time
        job->first_run = curr_time;
        job->completion_time = curr_time + job->time_remaining;   

        // update the current time
        curr_time = job->completion_time;     
    }
    
    return results;
}

void sort_arrival_and_time_remaining (struct thread_info* jobs_array, int numJobs)
/*Sorts jobs by arrival time using bubble sort. 
  Sorts jobs with the same arrival time by using their time_remaining*/
{
    for (int i = 0; i < numJobs - 1; i++) 
    {
        for (int j = 0; j < numJobs - i - 1; j++) 
        {   
            //sort by arrival
            if (jobs_array[j].arrival_time > jobs_array[j+1].arrival_time) 
            {
                // swap the jobs
                struct thread_info temp = jobs_array[j];
                jobs_array[j] = jobs_array[j+1];
                jobs_array[j+1] = temp;
            }

            else if (jobs_array[j].arrival_time == jobs_array[j+1].arrival_time) 
            {   
                //sorting jobs with the same arrival time by time remaining
                if (jobs_array[j].time_remaining > jobs_array[j+1].time_remaining) 
                {
                    struct thread_info temp = jobs_array[j];
                    jobs_array[j] = jobs_array[j+1];
                    jobs_array[j+1] = temp;
                }
            }
        }
    }
}

void sjf_scheduler(struct thread_info *jobs_array) {
/* Schedules jobs based on the Shortest Job First method.*/
    
    int numJobs = 5;
    int curr_time = 0;

    sort_arrival_and_time_remaining(jobs_array, numJobs);

    for (int i = 0; i < numJobs; i++) 
    {
        struct thread_info* job = &jobs_array[i];

        //wait for the job to arrive, if the time is 
        //less than the next arrival time
        if (curr_time < job->arrival_time) 
        {
            curr_time = job->arrival_time;
        }

        //job is finished
        job->first_run = curr_time;
        job->completion_time = curr_time + job->time_remaining;

        //update how lob it took to complete
        curr_time = job->completion_time;
    }
}

void setup(struct thread_info *jobs, int * job_info)
{
    int j = 0;
    for (int i = 0; i < 10; i = i + 2)
    {
        jobs[j].job_ID = j+1;
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
    int job_starter_info[10] = {0, 13, 0, 5, 3, 3, 4, 18, 19, 10};
    
    setup(job_list, job_starter_info);
    
    printf("Running FIFO tests\n");
    struct thread_info *fifo_results = fifo_not_in_place(job_list);
    responsetime(fifo_results, 5);
    turnaroundtime(fifo_results, 5);

    for (int i = 0; i < 5; i++)
    {
        printf("Job %d expected turnaround time: %d, actual turnaround time: %d\n",
                (i+1), job_expected_times_fifo[i][0], fifo_results[i].turnaround_time);
        printf("Job %d expected response time: %d, actual response time: %d\n\n",
                (i+1), job_expected_times_fifo[i][1], fifo_results[i].response_time);
    }

    printf("Running SFJ tests\n");
    sjf_scheduler(job_list);
    responsetime(job_list, 5);
    turnaroundtime(job_list, 5);

    for (int i = 0; i < 5; i++)
    {
        printf("Job %d expected turnaround time: %d, actual turnaround time: %d\n",
                (i+1), job_expected_times_sfj[i][0], job_list[i].turnaround_time);
        printf("Job %d expected response time: %d, actual response time: %d\n\n",
                (i+1), job_expected_times_sfj[i][1], job_list[i].response_time);
    }

    return 0;
}
