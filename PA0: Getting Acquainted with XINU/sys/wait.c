/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{
	proc_timer[currpid][a_wait].name="wait";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_wait].ctr+=1;
			proc_timer[currpid][a_wait].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_wait].ctr+=1;
			proc_timer[currpid][a_wait].timer+=ctr1000-start_time;
		}
		return pptr->pwaitret;
	}
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_wait].ctr+=1;
		proc_timer[currpid][a_wait].timer+=ctr1000-start_time;
	}
	return(OK);
}
