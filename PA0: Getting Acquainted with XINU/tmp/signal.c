/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{
	proc_timer[currpid][a_signal].name="signal";
	int start_time = ctr1000;
	STATWORD ps;    
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_signal].ctr+=1;
			proc_timer[currpid][a_signal].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_signal].ctr+=1;
		proc_timer[currpid][a_signal].timer+=ctr1000-start_time;
	}
	return(OK);
}
