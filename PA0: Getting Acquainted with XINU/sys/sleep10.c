/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(int n)
{
	proc_timer[currpid][a_sleep10].name="sleep10";
	int start_time=ctr1000;
	STATWORD ps;    
	if (n < 0  || clkruns==0) {
	        if (start_flag) {
                	proc_timer[currpid][a_sleep10].ctr+=1;
                	proc_timer[currpid][a_sleep10].timer+=ctr1000-start_time;
		} 
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if (start_flag) {
                proc_timer[currpid][a_sleep10].ctr+=1;
                proc_timer[currpid][a_sleep10].timer+=ctr1000-start_time;
	}
	return(OK);
}
