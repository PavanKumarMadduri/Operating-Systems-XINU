/* recvclr.c - recvclr */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  recvclr  --  clear messages, returning waiting message (if any)
 *------------------------------------------------------------------------
 */
SYSCALL	recvclr()
{
	proc_timer[currpid][a_recvclr].name="recvclr";
	int start_time = ctr1000;
	STATWORD ps;    
	WORD	msg;

	disable(ps);
	if (proctab[currpid].phasmsg) {
		proctab[currpid].phasmsg = 0;
		msg = proctab[currpid].pmsg;
	} else
		msg = OK;
	restore(ps);
	if (start_flag) {
		proc_timer[currpid][a_recvclr].ctr+=1;
		proc_timer[currpid][a_recvclr].timer+=ctr1000-start_time;
	}
	return(msg);
}
