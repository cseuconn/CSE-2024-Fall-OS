//Universal Thread Struct, please use when developing your section!
struct thread_info {
    int job_ID;
    int arrival_time;
    int turnaround_time;
    int completion_time = 0;
    int time_remaining;
    int first_run;  
    int response_time;
};