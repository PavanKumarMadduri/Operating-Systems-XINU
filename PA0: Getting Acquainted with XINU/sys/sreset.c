/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL sreset(int sem, int count)
{
	proc_timer[currpid][a_sreset].name="sreset";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;
	int	pid;
	int	slist;

	disable(ps);
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_sreset].ctr+=1;
			proc_timer[currpid][a_sreset].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_sreset].ctr+=1;
		proc_timer[currpid][a_sreset].timer+=ctr1000-start_time;
	}
	return(OK);
}
