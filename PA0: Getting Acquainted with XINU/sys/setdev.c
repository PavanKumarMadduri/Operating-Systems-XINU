/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	proc_timer[currpid][a_setdev].name="setdev";
	int start_time = ctr1000;
	short	*nxtdev;

	if (isbadpid(pid)) {
		if (start_flag) {
			proc_timer[currpid][a_setdev].ctr+=1;
			proc_timer[currpid][a_setdev].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	if (start_flag) {
		proc_timer[currpid][a_setdev].ctr+=1;
		proc_timer[currpid][a_setdev].timer+=ctr1000-start_time;
	}
	return(OK);
}
