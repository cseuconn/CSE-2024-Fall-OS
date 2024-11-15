
/*
 sorts the jobs by arrival time --> in place
*/
void sort_arrival_time(struct thread_info* jobs_array, int numJobs);


/*
 runs the first-in-first-out scheduler in place
*/
void fifo_in_place(struct thread_info* jobs_array);


/*
 runs the first-in-first-out scheduler not in place
 returns a new array
*/
struct thread_info* fifo_not_in_place(struct thread_info* jobs_array);