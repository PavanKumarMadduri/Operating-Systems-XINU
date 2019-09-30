/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	/* Age Scheduler Code */
	if (sched_uler == AGESCHED) {
		agesched();
	}
	else if (sched_uler == LINUXSCHED) {
		linuxsched();
	}
	else {
		oresched();
	}
	return OK;
}
