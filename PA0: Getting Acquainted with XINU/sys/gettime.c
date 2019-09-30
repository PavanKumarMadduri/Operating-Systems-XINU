/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <proc.h>
#include <lab0.h>
extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
	proc_timer[currpid][a_gettime].name="gettime";
    /* long	now; */
	int start_time = ctr1000;
	/* FIXME -- no getutim */
	if (start_flag) {
		proc_timer[currpid][a_gettime].ctr+=1;
		proc_timer[currpid][a_gettime].timer+=ctr1000-start_time;
	}
    return OK;
}
