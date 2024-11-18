/*
 * Scheduler.
 *
 * The default scheduler is very simple, just a round-robin run queue.
 * You'll want to improve it.
 */

#include <types.h>
#include <lib.h>
#include <scheduler.h>
#include <thread.h>
#include <machine/spl.h>
#include <scheduler_cfs.h>
#include <rbtree.h>
#define SCHEDULER CFS
/*
 *  Scheduler data
 */
// Queue of runnable threads
//static struct queue *runqueue;
//struct rbNode* root = NULL; //static struct rbNode* root;
/*
 * Setup function
 */
void
scheduler_bootstrap_cfs(void) {}

/*
 * Ensure space for handling at least NTHREADS threads.
 * This is done only to ensure that make_runnable() does not fail -
 * if you change the scheduler to not require space outside the 
 * thread structure, for instance, this function can reasonably
 * do nothing.
 */
int
scheduler_preallocate_cfs(int nthreads) { return 0; }

/*
 * This is called during panic shutdown to dispose of threads other
 * than the one invoking panic. We drop them on the floor instead of
 * cleaning them up properly; since we're about to go down it doesn't
 * really matter, and freeing everything might cause further panics.
 */
void
scheduler_killall_cfs(void)
{
	assert(curspl>0);
	while (root != NULL) {
		struct thread *t = root->data; //q_remhead(runqueue);
		kprintf("scheduler: Dropping thread %s.\n", t->t_name);
		deletion(root->data);
	}
}

/*
 * Cleanup function.
 *
 * The queue objects to being destroyed if it's got stuff in it.
 * Use scheduler_killall to make sure this is the case. During
 * ordinary shutdown, normally it should be.
 */
void
scheduler_shutdown_cfs(void)
{
	scheduler_killall();

	assert(curspl>0);
	//q_destroy(runqueue);
	root = NULL;
}

/*
 * Actual scheduler. Returns the next thread to run.  Calls cpu_idle()
 * if there's nothing ready. (Note: cpu_idle must be called in a loop
 * until something's ready - it doesn't know whether the things that
 * wake it up are going to make a thread runnable or not.) 
 */
struct thread *
scheduler_cfs(void)
{
	// meant to be called with interrupts off
	assert(curspl>0);
	
	while (root == NULL) {
		cpu_idle();
	}

	// You can actually uncomment this to see what the scheduler's
	// doing - even this deep inside thread code, the console
	// still works. However, the amount of text printed is
	// prohibitive.
	// 
	//print_run_queue();
	struct thread* next = leftmost(root)->data;
	deletion(leftmost(root)->data);
	return next;
}

/* 
 * Make a thread runnable.
 * With the base scheduler, just add it to the end of the run queue.
 */
int
make_runnable_cfs(struct thread *t)
{
	// meant to be called with interrupts off
	assert(curspl>0);
	insertion(t);
	return 0;
}

/*
 * Debugging function to dump the run queue.
 */
void
print_run_queue_cfs(void)
{
	kprintf("TODO\n");
}

