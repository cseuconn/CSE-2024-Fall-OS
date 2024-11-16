//Brian Platas Gutierrez
#include <thread.h>
#include <main.c>


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
    
    int numJobs = sizeof(jobs_array) / sizeof(jobs_array[0]);
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
        job->completion_time = curr_time + job->time_remaining;

        //update how lob it took to complete
        curr_time = job->completion_time;
    }
}