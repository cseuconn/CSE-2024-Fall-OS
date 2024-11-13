#include <types.h>
#include <lib.h>
#include <scheduler.h>
#include <thread.h>
#include <machine/spl.h>
#include <scheduler_fifo.h>
#include <queue.h>
#include <curthread.h>
//static struct queue *fifo_runqueue;

void scheduler_bootstrap_fifo(void)
{
    fifo_runqueue = q_create(32);
    if (fifo_runqueue == NULL) {
        panic("scheduler: Could not create FIFO run queue\n");
    }
}

int scheduler_preallocate_fifo(int nthreads)
{
    assert(curspl > 0);
    return q_preallocate(fifo_runqueue, nthreads);
}

void scheduler_killall_fifo(void)
{
    assert(curspl > 0);
    while (!q_empty(fifo_runqueue)) {
        struct thread *t = q_remhead(fifo_runqueue);
        kprintf("scheduler: Dropping thread %s.\n", t->t_name);
    }
}

void scheduler_shutdown_fifo(void)
{
    scheduler_killall_fifo();
    assert(curspl > 0);
    q_destroy(fifo_runqueue);
    fifo_runqueue = NULL;
}

struct thread *scheduler_fifo(void)
{
    assert(curspl > 0);
    while (q_empty(fifo_runqueue)) {
        cpu_idle();
    }
    //if (curthread) return curthread;
    return q_remhead(fifo_runqueue);
}

int make_runnable_fifo(struct thread *t)
{
    assert(curspl > 0);
    //curthread = t;
    if ((t->cpu_secs * 1000000000 + t->cpu_nsecs) > 0) return q_addhead(fifo_runqueue, t);
    else return q_addtail(fifo_runqueue, t);
}

void print_run_queue_fifo(void)
{
    int spl = splhigh();
    int i, k = 0;
    i = q_getstart(fifo_runqueue);
    while (i != q_getend(fifo_runqueue)) {
        struct thread *t = q_getguy(fifo_runqueue, i);
        kprintf("  %2d: %s %p\n", k, t->t_name, t->t_sleepaddr);
        i = (i + 1) % q_getsize(fifo_runqueue);
        k++;
    }
    splx(spl);
}
