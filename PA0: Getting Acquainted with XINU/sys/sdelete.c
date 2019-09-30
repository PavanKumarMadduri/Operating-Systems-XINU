/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(int sem)
{
	proc_timer[currpid][a_sdelete].name="sdelete";
	int start_time = ctr1000;
	STATWORD ps;    
	int	pid;
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_sdelete].ctr+=1;
			proc_timer[currpid][a_sdelete].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_sdelete].ctr+=1;
		proc_timer[currpid][a_sdelete].timer+=ctr1000-start_time;
	}
	return(OK);
}
