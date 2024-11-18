#include <types.h>
#include <lib.h>
#include <scheduler.h>
#include <thread.h>
#include <machine/spl.h>
#include <scheduler_rr.h>
#include <queue.h>
//#define SCHEDULER RR

//static struct queue *runqueue;
//runqueue = NULL;
void
scheduler_bootstrap_rr(void)
{
        runqueue = q_create(32);
        if (runqueue == NULL) {
                panic("scheduler: Could not create run queue\n");
        }
}

/*
 * Ensure space for handling at least NTHREADS threads.
 * This is done only to ensure that make_runnable() does not fail -
 * if you change the scheduler to not require space outside the 
 * thread structure, for instance, this function can reasonably
 * do nothing.
 */
int
scheduler_preallocate_rr(int nthreads)
{
        assert(curspl>0);
        return q_preallocate(runqueue, nthreads);
}

/*
 * This is called during panic shutdown to dispose of threads other
 * than the one invoking panic. We drop them on the floor instead of
 * cleaning them up properly; since we're about to go down it doesn't
 * really matter, and freeing everything might cause further panics.
 */
void
scheduler_killall_rr(void)
{
        assert(curspl>0);
        while (!q_empty(runqueue)) {
                struct thread *t = q_remhead(runqueue);
                kprintf("scheduler: Dropping thread %s.\n", t->t_name);
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
scheduler_shutdown_rr(void)
{
        scheduler_killall();

        assert(curspl>0);
        q_destroy(runqueue);
        runqueue = NULL;
}

/*
 * Actual scheduler. Returns the next thread to run.  Calls cpu_idle()
 * if there's nothing ready. (Note: cpu_idle must be called in a loop
 * until something's ready - it doesn't know whether the things that
 * wake it up are going to make a thread runnable or not.) 
 */

struct thread *
scheduler_rr(void)
{
        // meant to be called with interrupts off
        assert(curspl>0);

        while (q_empty(runqueue)) {
                cpu_idle();
        }

        // You can actually uncomment this to see what the scheduler's
        // doing - even this deep inside thread code, the console
        // still works. However, the amount of text printed is
        // prohibitive.
        // 
        //print_run_queue();
        return q_remhead(runqueue);
        //inorderTraversal(root);
        //return q_remhead(runqueue);
}

/* 
 * Make a thread runnable.
 * With the base scheduler, just add it to the end of the run queue.
 */
int
make_runnable_rr(struct thread *t)
{
        // meant to be called with interrupts off
        assert(curspl>0);
//        insertion(t);
        //inorderTraversal(root);
        return q_addtail(runqueue, t);
}

/*
 * Debugging function to dump the run queue.
 */
void
print_run_queue_rr(void)
{
        /* Turn interrupts off so the whole list prints atomically. */
        int spl = splhigh();

        int i,k=0;
        i = q_getstart(runqueue);

        while (i!=q_getend(runqueue)) {
                struct thread *t = q_getguy(runqueue, i);
                kprintf("  %2d: %s %p\n", k, t->t_name, t->t_sleepaddr);
                i=(i+1)%q_getsize(runqueue);
                k++;
        }

        splx(spl);
}

