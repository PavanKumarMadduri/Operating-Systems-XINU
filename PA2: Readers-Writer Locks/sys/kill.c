/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	findlock(pid);	
	
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}

findlock(int f_pid) {
	int i;
	int item=dequeue(f_pid);
	struct lentry *j;
	for(i=0 ; i < NLOCKS; i++) {
		j=&locks[i];
		j->lproc[f_pid]=0;
		if (proctab[f_pid].proclock[i] !=0 && j->lproc[f_pid] == 0 ) { // To find all the locks that process is present in and has acquired. Exist - Blocked
//			kprintf("Lock ID - %d\t, Status - %d\n", i,proctab[f_pid].pstate);
			proctab[f_pid].proclock[i] =0;
			prioinherit(i);
		}
	}
}
