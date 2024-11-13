/*
 * Scheduler.
 *
 * The default scheduler is very simple, just a round-robin run queue.
 * You'll want to improve it.
 */

#include <types.h>
#include <lib.h>
#include <scheduler.h>
#include <scheduler_rr.h>
#include <scheduler_cfs.h>
#include <scheduler_fifo.h>
#include <thread.h>
#include <machine/spl.h>
#include <queue.h>
#include <rbtree.h>
/*
 *  Scheduler data
 */
#define SCHEDULER FIFO
// Queue of runnable threads
//static struct queue *runqueue;
struct rbNode* root = NULL; //static struct rbNode* root;
struct queue* runqueue = NULL;
struct queue* fifo_runqueue = NULL;
/*
 * Setup function
 */
void
scheduler_bootstrap(void)
{
	if (SCHEDULER == CFS) scheduler_bootstrap_cfs();
	else if (SCHEDULER == RR) scheduler_bootstrap_rr();
	else scheduler_bootstrap_fifo();
}

/*
 * Ensure space for handling at least NTHREADS threads.
 * This is done only to ensure that make_runnable() does not fail -
 * if you change the scheduler to not require space outside the 
 * thread structure, for instance, this function can reasonably
 * do nothing.
 */
int
scheduler_preallocate(int nthreads)
{
        if (SCHEDULER == CFS) return scheduler_preallocate_cfs(nthreads);
        else if (SCHEDULER == RR) return scheduler_preallocate_rr(nthreads);
	else scheduler_preallocate_fifo(nthreads);
}

/*
 * This is called during panic shutdown to dispose of threads other
 * than the one invoking panic. We drop them on the floor instead of
 * cleaning them up properly; since we're about to go down it doesn't
 * really matter, and freeing everything might cause further panics.
 */
void
scheduler_killall(void)
{
	if (SCHEDULER == CFS) scheduler_killall_cfs();
	else if (SCHEDULER == RR) scheduler_killall_rr();
	else scheduler_killall_fifo();
}

/*
 * Cleanup function.
 *
 * The queue objects to being destroyed if it's got stuff in it.
 * Use scheduler_killall to make sure this is the case. During
 * ordinary shutdown, normally it should be.
 */
void
scheduler_shutdown(void)
{
        if (SCHEDULER == CFS) scheduler_shutdown_cfs();
        else if (SCHEDULER == RR) scheduler_shutdown_rr();
	else scheduler_shutdown_fifo();
}

/*
 * Actual scheduler. Returns the next thread to run.  Calls cpu_idle()
 * if there's nothing ready. (Note: cpu_idle must be called in a loop
 * until something's ready - it doesn't know whether the things that
 * wake it up are going to make a thread runnable or not.) 
 */
struct thread *
scheduler(void)
{
	// meant to be called with interrupts off
        if (SCHEDULER == CFS) return scheduler_cfs();
        else if (SCHEDULER == RR) return scheduler_rr();
	else return scheduler_fifo();
}

/* 
 * Make a thread runnable.
 * With the base scheduler, just add it to the end of the run queue.
 */
int
make_runnable(struct thread *t)
{
	// meant to be called with interrupts off
        if (SCHEDULER == CFS) return make_runnable_cfs(t);
        else if (SCHEDULER == RR) return make_runnable_rr(t);
	else return make_runnable_fifo(t);
}

/*
 * Debugging function to dump the run queue.
 */
void
print_run_queue(void)
{
	/* Turn interrupts off so the whole list prints atomically. */
        if (SCHEDULER == CFS) print_run_queue_cfs();
        else if (SCHEDULER == RR) print_run_queue_rr();
	else print_run_queue_fifo();
}
