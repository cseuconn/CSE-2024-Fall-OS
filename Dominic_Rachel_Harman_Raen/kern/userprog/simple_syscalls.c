#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <lib.h>
#include <machine/spl.h>
#include <test.h>
#include <synch.h>
#include <thread.h>
#include <scheduler.h>
#include <dev.h>
#include <vfs.h>
#include <vm.h>
#include <syscall.h>
#include <version.h>
void sys__exit(int code)
{
	kprintf("%d", code);
	thread_exit();
}

int sys_printint(int c)
{
	kprintf("%d", c);
	if (!(c % 2)) return -1;
	return 0;
}

int sys_reversestring(const char* str, int len)
{
	int i = 0;
	for (i; i < len; i++)
	{
		kprintf("%c", str[len-1-i]);
	}
	if (!(len % 5)) return -1;
	return 0;
}
