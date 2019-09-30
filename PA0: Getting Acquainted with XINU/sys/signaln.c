/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(int sem, int count)
{
	proc_timer[currpid][a_signaln].name="signaln";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_signaln].ctr+=1;
			proc_timer[currpid][a_signaln].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_signaln].ctr+=1;
		proc_timer[currpid][a_signaln].timer+=ctr1000-start_time;
	}
	return(OK);
}
