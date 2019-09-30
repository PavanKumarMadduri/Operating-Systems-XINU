#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lock.h>

transitivity(int pid, int new_pri) {
	int i,k, new_prio;
	struct lentry *j;
        for(i=0 ; i < NLOCKS; i++) {
                j=&locks[i];
                if (proctab[pid].proclock[i] !=0 && j->lproc[pid] == 0 ) { // To find all the locks that process is present in and has acquired. Exist - Blocked
			for (k=0; k < NPROC; k++) {
				if (j->lproc[k] !=0 && j->lprio < new_pri) {
					new_prio=chprio(k,new_pri);
				}
			}
		}
	}
}
prioinherit(int ldes1) {

	struct lentry *lptr;
	struct pentry *pptr;
	int i,proc,new_prio;

	lptr=&locks[ldes1];
	proc=q[lptr->lqtail].qprev;
	lptr->lprio=0;
	while ( proc < NPROC ) {
		if ( lptr->lprio < proctab[proc].pprio ) {
			lptr->lprio=proctab[proc].pprio;
		}
		proc=q[proc].qprev;
	}
	for (i=0 ; i < NPROC; i++) {
		if (lptr->lproc[i] != 0 && lptr->lprio==0) {
			proctab[i].pprio=proctab[i].pinh;
			new_prio=proctab[i].pinh;
			transitivity(i,new_prio);
		}
		if ( lptr->lproc[i] != 0 && proctab[i].pinh < lptr->lprio ) {
			proctab[i].pprio=lptr->lprio;
			new_prio=lptr->lprio;
			transitivity(i,new_prio);
		}
	}
}


int lock (int ldes1, int type, int priority) {

	extern int ctr1000;	

	STATWORD ps;
	struct lentry *lptr;
	struct pentry *pptr;
	int l_next;

	lptr=&locks[ldes1];

	disable(ps);

	if ((pptr = &proctab[currpid])->pwaitret == DELETED ) {
		restore(ps);
		return(SYSERR);
	}

	if (type != READ && type != WRITE) {
		restore(ps);
		return(SYSERR);
	}

	if ( isbadlock(ldes1) || locks[ldes1].lstate==LFREE ) {
		restore(ps);
		return(SYSERR);
	}
	if ( lptr->ltype == 0 ) {
		lptr->ltype=type;
		proctab[currpid].proclock[ldes1]=type;
		lptr->lproc[currpid]=type;
		lptr->lckcnt=type;
		restore(ps);
		return(OK);
	}
	else if ( lptr->ltype == READ ) {
		if ( type == READ ) {
			if (nonempty(lptr->lqhead)) {
				l_next=q[lptr->lqtail].qprev;
				while( l_next < NPROC ) {
					if (proctab[l_next].proclock[ldes1] == WRITE ) {
						if ( priority > q[l_next].qkey ) {
							proctab[currpid].proclock[ldes1]=READ;
							lptr->lproc[currpid]=type;
							lptr->lckcnt+=type;
							restore(ps);
							return(OK);
						}
						else {
							(pptr = &proctab[currpid])->pstate = PRWAIT;
							(pptr = &proctab[currpid])->pclock[ldes1] = ctr1000;
							(pptr = &proctab[currpid])->proclock[ldes1]=type;
							(pptr = &proctab[currpid])->lockid=ldes1;
							(pptr = &proctab[currpid])->ptype=type;
							insert(currpid,lptr->lqhead, priority);
							(pptr = &proctab[currpid])->pwaitret = OK;
							prioinherit(ldes1);
							resched();
							restore(ps);
							return pptr->pwaitret;
						}
					}
					l_next=q[l_next].qprev;
				}
			}
			else {
				proctab[currpid].proclock[ldes1]=READ;
				lptr->lproc[currpid]=type;
				lptr->lckcnt+=type;
				restore(ps);
				return(OK);
			}
		}
		else {
			(pptr = &proctab[currpid])->pstate = PRWAIT;
			(pptr = &proctab[currpid])->pclock[ldes1] = ctr1000;
			(pptr = &proctab[currpid])->proclock[ldes1]=type;
			(pptr = &proctab[currpid])->lockid=ldes1;
			(pptr = &proctab[currpid])->ptype=type;
			insert(currpid,lptr->lqhead, priority);
			(pptr = &proctab[currpid])->pwaitret = OK;
			prioinherit(ldes1);
			resched();
			restore(ps);
			return pptr->pwaitret;
		}
	}
	
	else {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		(pptr = &proctab[currpid])->pclock[ldes1] = ctr1000;
		(pptr = &proctab[currpid])->proclock[ldes1]=type;
		(pptr = &proctab[currpid])->lockid=ldes1;
		(pptr = &proctab[currpid])->ptype=type;
		insert(currpid,lptr->lqhead, priority);
		(pptr = &proctab[currpid])->pwaitret = OK;
		prioinherit(ldes1);
		resched();
		restore(ps);
		return pptr->pwaitret;
	}

}
