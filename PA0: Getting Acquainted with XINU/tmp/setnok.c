/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{
	proc_timer[currpid][a_setnok].name="setnok";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_setnok].ctr+=1;
			proc_timer[currpid][a_setnok].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_setnok].ctr+=1;
		proc_timer[currpid][a_setnok].timer+=ctr1000-start_time;
	}
	return(OK);
}
