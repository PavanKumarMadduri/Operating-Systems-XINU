/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
proc_timer[currpid][a_scount].name="scount";
int start_time = ctr1000;
extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		if (start_flag) {
			proc_timer[currpid][a_scount].ctr+=1;
			proc_timer[currpid][a_scount].timer+=ctr1000-start_time;
		}
		return(SYSERR);
	}
	if (start_flag) {
		proc_timer[currpid][a_scount].ctr+=1;
		proc_timer[currpid][a_scount].timer+=ctr1000-start_time;
	}
	return(semaph[sem].semcnt);
}
