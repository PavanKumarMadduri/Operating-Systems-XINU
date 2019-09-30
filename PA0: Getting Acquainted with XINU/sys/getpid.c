/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
	proc_timer[currpid][a_getpid].name="getpid";
	int start_time = ctr1000;
	if (start_flag) {
		proc_timer[currpid][a_getpid].ctr+=1;
		proc_timer[currpid][a_getpid].timer+=ctr1000-start_time;
	}
	return(currpid);
}
