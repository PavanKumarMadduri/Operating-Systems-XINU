#include <stdio.h>
#include <kernel.h>
#include <lock.h>
#include <proc.h>

struct  lentry  locks[NLOCKS];

linit() {
	int l,n;
	struct  lentry  *lptr;
	for (l=0 ; l<NLOCKS ; l++) {      /* initialize locks */
		(lptr = &locks[l])->lstate = LFREE;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
		lptr->ltype=0;
		lptr->lprio=0;
		lptr->lckcnt=0;
		for (n=0 ; n<NPROC ; n++) {
			lptr->lproc[n]=0;
		}
	}
}
