/*
 * Synchronization test code.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <test.h>
#include <clock.h>
#include <curthread.h>
#include <rbtree.h>
//#include <unistd.h>
#define NTHREADS      32
#define ALL_AT_ONCE 0
#define RANDOM 1
#define IO_CPU 2
#define LONG_SHORT 3
#define LONG_FIRST 4
#define THREAD_SCHEDULE RANDOM
#define LOAD LONG_SHORT
#define RANDOM_ORDER 1
static struct semaphore *donesem;

static
void
inititems(void)
{
	if (donesem==NULL) {
		donesem = sem_create("donesem", 0);
		if (donesem == NULL) {
			panic("schedtest: sem_create failed\n");
		}
	}
}

static
void
schedtestthread(void *junk, unsigned long num)
{
	int j, k;
	if ((num % 4 < 2 && LOAD != LONG_FIRST) || (num < (NTHREADS / 2) && LOAD == LONG_FIRST)) {
		if (LOAD == IO_CPU) {
			for (k = 0; k < 10; k++) {
				for (j = 0; j < 10000; j++){}
				csleep(1);
			}
		}
		else if (LOAD == LONG_SHORT) for (k = 0; k < 25000; k++) {}
	}
	else {
		int upper = (LOAD == IO_CPU ? 350000 : 250000);
		for (j = 0; j < upper; j++) {}
	}
	V(donesem);
}


int
schedtest(int nargs, char **args)
{
	int i, result, j, n;

	(void)nargs;
	(void)args;
	inititems();
	kprintf("Starting scheduler test...\n");

	for (i=0; i<NTHREADS; i++) {
		if (RANDOM_ORDER){n = random() % NTHREADS;}
		else{n = i;}

		result = thread_fork("schedtest", NULL, n, schedtestthread,
				     NULL);
		if (result) {
			panic("schedtest: thread_fork failed: %s %s\n",
			      strerror(result), i);
		}
		if (THREAD_SCHEDULE == RANDOM) for (j = 0; j < random() % 10000; j++){} 
	}
	for (i=0; i<NTHREADS; i++) {
		P(donesem);
	}

	kprintf("scheduler test done.\n");

	return 0;
}
