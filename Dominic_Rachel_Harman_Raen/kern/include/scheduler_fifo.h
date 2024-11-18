#ifndef _SCHEDULER_FIFO_H_
#define _SCHEDULER_FIFO_H_

#include <queue.h>  // Include queue definitions if necessary

/*
 * FIFO Scheduler-related function declarations.
 */

struct thread;
extern struct queue *fifo_runqueue;
struct thread *scheduler_fifo(void);
int make_runnable_fifo(struct thread *t);

void print_run_queue_fifo(void);

void scheduler_bootstrap_fifo(void);
int scheduler_preallocate_fifo(int numthreads);
void scheduler_killall_fifo(void);
void scheduler_shutdown_fifo(void);

#endif /* _SCHEDULER_FIFO_H_ */







