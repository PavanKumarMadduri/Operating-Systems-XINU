/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	proc_timer[currpid][a_getprio].name="getprio";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_getprio].ctr+=1;
			proc_timer[currpid][a_getprio].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	restore(ps);
	if (start_flag) {
        	proc_timer[currpid][a_getprio].ctr+=1;
                proc_timer[currpid][a_getprio].timer+=ctr1000-start_time;
	}
	return(pptr->pprio);
}
