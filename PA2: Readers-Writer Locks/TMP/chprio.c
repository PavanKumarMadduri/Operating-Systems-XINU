/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	pptr->pprio = newprio;
	int i,j;
	struct lentry *lptr;
	for (i=0; i < NLOCKS; i++ ) {
		lptr=&locks[i];
		if ( proctab[pid].proclock[i] !=0 && lptr->lproc[pid] ==0 ) {
			prioinherit(i);
		}	
	}
	restore(ps);
	return(newprio);
}
