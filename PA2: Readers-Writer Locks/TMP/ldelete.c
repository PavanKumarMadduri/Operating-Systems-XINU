#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <lock.h>
#include <proc.h>
#include <stdio.h>

int ldelete (int lockdescriptor) {

	STATWORD ps;    
	int	pid,l;
	struct	lentry	*lptr;

	disable(ps);
	if (isbadlock(lockdescriptor) || locks[lockdescriptor].lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &locks[lockdescriptor];
	lptr->lstate = LFREE;
	lptr->ltype=0;
	lptr->lckcnt=0;
	for (l=0 ; l < NPROC ; l++) {
		lptr->lproc[l]=0;
	}

	if (nonempty(lptr->lqhead)) {
		while( (pid=getfirst(lptr->lqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	return(OK);

}
