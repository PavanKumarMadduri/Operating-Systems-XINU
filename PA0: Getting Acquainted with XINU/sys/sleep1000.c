/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep1000(int n)
{
	proc_timer[currpid][a_sleep1000].name="sleep1000";
	int start_time = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0) {
		if (start_flag) {
			proc_timer[currpid][a_sleep1000].ctr+=1;
			proc_timer[currpid][a_sleep1000].timer+=ctr1000-start_time;
		}
	         return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if (start_flag) {
        	proc_timer[currpid][a_sleep1000].ctr+=1;
                proc_timer[currpid][a_sleep1000].timer+=ctr1000-start_time;
         } 
	return(OK);
}
