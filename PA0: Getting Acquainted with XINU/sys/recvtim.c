/* recvtim.c - recvtim */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  recvtim  -  wait to receive a message or timeout and return result
 *------------------------------------------------------------------------
 */
SYSCALL	recvtim(int maxwait)
{
	proc_timer[currpid][a_recvtim].name="recvtim";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;
	int	msg;

	if (maxwait<0 || clkruns == 0) {
		if (start_flag) {
			proc_timer[currpid][a_recvtim].ctr+=1;
			proc_timer[currpid][a_recvtim].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	disable(ps);
	pptr = &proctab[currpid];
	if ( !pptr->phasmsg ) {		/* if no message, wait		*/
	        insertd(currpid, clockq, maxwait*1000);
		slnempty = TRUE;
		sltop = (int *)&q[q[clockq].qnext].qkey;
	        pptr->pstate = PRTRECV;
		resched();
	}
	if ( pptr->phasmsg ) {
		msg = pptr->pmsg;	/* msg. arrived => retrieve it	*/
		pptr->phasmsg = FALSE;
	} else {			/* still no message => TIMEOUT	*/
		msg = TIMEOUT;
	}
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_recvtim].ctr+=1;
		proc_timer[currpid][a_recvtim].timer+=ctr1000-start_time;
	}
	return(msg);

}
