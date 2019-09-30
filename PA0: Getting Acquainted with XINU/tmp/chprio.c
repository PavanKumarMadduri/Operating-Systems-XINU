/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	proc_timer[currpid][a_chprio].name="chprio";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_chprio].ctr+=1;
			proc_timer[currpid][a_chprio].timer+=ctr1000-start_time;
		}

		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_chprio].ctr+=1;
		proc_timer[currpid][a_chprio].timer+=ctr1000-start_time;
	}
	return(newprio);
}
