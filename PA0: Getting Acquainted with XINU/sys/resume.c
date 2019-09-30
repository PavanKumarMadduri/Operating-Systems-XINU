/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(int pid)
{
	proc_timer[currpid][a_resume].name="resume";
	int start_time = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
		if (start_flag) {
			proc_timer[currpid][a_resume].ctr+=1;
			proc_timer[currpid][a_resume].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_resume].ctr+=1;
		proc_timer[currpid][a_resume].timer+=ctr1000-start_time;
	}
	return(prio);
}
