/* linuxsched.c  -  linuxsched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * linuxsched  --  reschedule processor to highest goodness ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
extern int epoch;
int linuxsched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	
	int pro, good, ctr=0, max=-5, maxpid=currpid;

//	kprintf("%s\t%d\t%d\t%d\t%d\n", proctab[currpid].pname,epoch,preempt,proctab[currpid].quantum,proctab[currpid].goodness);
	epoch=epoch-preempt;

	if (preempt <= 0) {
		epoch=epoch-preempt;
		proctab[currpid].goodness=0;
		proctab[currpid].unused_quantum=0;
	}
	else {
		epoch=epoch;
		proctab[currpid].goodness=proctab[currpid].pprio+proctab[currpid].unused_quantum;
		proctab[currpid].unused_quantum=preempt;
	}	

//	kprintf("%s\t%d\t%d\t%d\t%d\n", proctab[currpid].pname,epoch,preempt,proctab[currpid].quantum,proctab[currpid].goodness);

        good=q[rdytail].qprev;
        while (good!=rdyhead && good!=0) {
                if (proctab[good].goodness>max) {
                        max=proctab[good].goodness;
                        maxpid=good;
                }
                good=q[good].qprev;
        }


        if (epoch<=0 || max ==0) {
                for (pro=1; pro < NPROC; pro++) {
                        if (proctab[pro].pstate==PRCURR || proctab[pro].pstate==PRREADY || proctab[pro].pstate==PRSLEEP || proctab[pro].pstate==PRWAIT || proctab[pro].pstate ==PRSUSP) {
				proctab[pro].goodness=proctab[pro].pprio+proctab[pro].unused_quantum;
				proctab[pro].quantum=proctab[pro].pprio+proctab[pro].unused_quantum/2;
				epoch = epoch + proctab[pro].quantum;
//				kprintf("%s\t%d\t%d\t%d\t%d\n", proctab[pro].pname,epoch,preempt,proctab[pro].quantum,proctab[pro].goodness);
				ctr=ctr+1;
                        }
                }
        }

//	kprintf("Count:%d\n",ctr);
//	kprintf("%s\t%d\t%d\t%d\t%d\n", proctab[currpid].pname,epoch,preempt,proctab[currpid].quantum,proctab[currpid].goodness);
	

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (max<proctab[currpid].goodness)) {
		preempt = proctab[currpid].quantum;
		return(OK);
	}
//	 kprintf("-----------------------------------------\n");	
	/* force context switch */	

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	} 


	/* remove highest priority process at end of ready list */
	nptr = &proctab[ currpid = getlast(rdytail) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
#ifdef	RTCLOCK
	preempt = proctab[currpid].quantum;		/* reset preemption counter	*/
#endif
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	/* The OLD process returns here when resumed. */
	return OK;
}
