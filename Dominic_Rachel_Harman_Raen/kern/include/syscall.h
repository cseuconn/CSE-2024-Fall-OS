#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
void sys__exit(int code);
int sys_printint(int c);
int sys_reversestring(const char* str, int len);
#endif /* _SYSCALL_H_ */
