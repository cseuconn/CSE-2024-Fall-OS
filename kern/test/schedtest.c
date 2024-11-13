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
<<<<<<< HEAD
#define LONG_FIRST 2
=======
#define IO_CPU 2
#define LONG_SHORT 3
#define LONG_FIRST 4
#define THREAD_SCHEDULE RANDOM
#define LOAD LONG_SHORT
#define RANDOM_ORDER 1
static struct semaphore *donesem;
>>>>>>> d70fea0d29bf77bc5ab20fbc0e0305d6c6c1e0bf

#define IO_CPU 3
#define LONG_SHORT 4
#define PRIORITIES 5
#define THREAD_SCHEDULE ALL_AT_ONCE
#define LOAD PRIORITIES
char long_first_threads[32] = {1,5,9,13,17,21,25,29,0,4,8,12,16,20,24,28,2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31};

static
void
schedtestthread(void *junk, unsigned long num)
{
	int j, k;
<<<<<<< HEAD
	switch (LOAD) {
		case LONG_SHORT:
			if (num % 4 < 2) for (k = 0; k < 250000; k++) {}
			else for (k = 0; k < 25000; k++) {}
			break;
		case IO_CPU:
			if (num % 4 < 2) for (k = 0; k < 10; k++)  {
				for (j = 0; j < 10000; j++) {}
=======
	if ((num % 4 < 2 && LOAD != LONG_FIRST) || (num < (NTHREADS / 2) && LOAD == LONG_FIRST)) {
		if (LOAD == IO_CPU) {
			for (k = 0; k < 10; k++) {
				for (j = 0; j < 10000; j++){}
>>>>>>> d70fea0d29bf77bc5ab20fbc0e0305d6c6c1e0bf
				csleep(1);
			}
			else for (j = 0; j < 350000; j++) {}
			break;
		case PRIORITIES:
			for (j = 0; j < 350000; j++){}
			break;
	}
}


int
schedtest(int nargs, char **args)
{
	int i, result, j, n;

	(void)nargs;
	(void)args;
	kprintf("Starting scheduler test...\n");

	for (i=0; i<NTHREADS; i++) {
<<<<<<< HEAD
		switch (THREAD_SCHEDULE) {
			case RANDOM:
				n = random() % NTHREADS;
				for (j = 0; j < random() % 10000; j++){}
				break;
			case ALL_AT_ONCE:
				n = i;
				break;
			case LONG_FIRST:
				n = long_first_threads[i];
				for (j = 0; j < 10000; j++) {}
				break;
		}
		if (LOAD == PRIORITIES) result = thread_fork("schedtest", NULL, n, schedtestthread,
				     NULL, 1+(n%2));
		else result = thread_fork("schedtest", NULL, n, schedtestthread, NULL, 1+(n%2));

=======
		if (RANDOM_ORDER){n = random() % NTHREADS;}
		else{n = i;}

		result = thread_fork("schedtest", NULL, n, schedtestthread,
				     NULL);
>>>>>>> d70fea0d29bf77bc5ab20fbc0e0305d6c6c1e0bf
		if (result) {
			panic("schedtest: thread_fork failed: %s %s\n",
			      strerror(result), i);
		}
	}
	return 0;
}
