#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <q.h>
#include <stdio.h>

int releaseall (int numlocks, long ldes) {
	
	STATWORD ps;
	extern int ctr1000;
	struct lentry *lptr;
	struct pentry *pptr;
	int pro,status[numlocks],d=0, wr, exist=0,lprio=0, high=0;
	int curr_time = ctr1000;
	unsigned long	*c;

	c = (unsigned long *)(&ldes) + (numlocks-1);
	disable(ps);
	for (; numlocks > 0 ; numlocks--) {
		lptr = &locks[*c];
// Check if the Lock is bad
		if ( isbadlock(*c) || locks[*c].lstate==LFREE || proctab[currpid].proclock[*c] == 0 ) {
			status[numlocks] = 1;
		}
		else {
// Finding the PID & Priority of Highest Write Process Present in the Lock Queue 

			wr=q[lptr->lqtail].qprev;
			while ( wr < NPROC ) {
				if ( proctab[wr].proclock[*c] == WRITE ) {
					exist=1;
					if ( lprio < q[wr].qkey ) {
						high=wr;
					}		
				}
				wr=q[wr].qprev;
			}

			
/*			for (wr=0 ; wr < NPROC ; wr++ ) {
				if ( lptr->lproc[wr] == WRITE && wr != currpid ) {
					exist = 1;
					if ( lprio < q[wr].qkey ) {
						lprio=q[wr].qkey;
						high=wr;
					}
				}
			}
*/
			if ( lptr->lckcnt < 4 ) { // Check if there are more than one process holding the Lock
				if (nonempty(lptr->lqhead)) { // Check if the Lock queue is empty
					if ( exist == 1 ) { // Check if a WRITE Process present in the Lock queue
						pro=q[lptr->lqtail].qprev;
						if ( proctab[pro].proclock[*c] == READ ) {
							if ( q[pro].qkey == q[high].qkey && ( (curr_time - proctab[pro].pclock[*c]) < 600 || (curr_time - proctab[pro].pclock[*c]) == 600 ) ) {
								status[numlocks] = 0;
								lptr->lckcnt=lptr->lckcnt-proctab[currpid].proclock[*c]+proctab[high].proclock[*c];
								lptr->ltype=proctab[high].proclock[*c];
								proctab[currpid].proclock[*c]=0;
								dequeue(high);
								q[high].qkey=proctab[high].pprio;
								ready(high, RESCHNO);
							}
							else {  
								pro=q[high].qnext;
								lptr->ltype=READ;
								lptr->lckcnt-=proctab[currpid].proclock[*c];
								while ( pro < NPROC ) {
									proctab[currpid].proclock[*c]=0;
									if (proctab[pro].proclock[*c] == READ ) {
										lptr->lproc[pro]=0;
										lptr->lckcnt+=proctab[pro].proclock[*c];
										dequeue(pro);
										q[pro].qkey=proctab[pro].pprio;
										ready(pro, RESCHNO);
									}
								pro=q[pro].qnext;
								}
								status[numlocks] = 0;
							}
						}
						else {
							status[numlocks] = 0;
							lptr->lckcnt=lptr->lckcnt-proctab[currpid].proclock[*c]+proctab[pro].proclock[*c];
							lptr->lproc[pro]=proctab[currpid].proclock[*c];
							lptr->ltype=WRITE;
							proctab[currpid].proclock[*c]=0;
							dequeue(pro);
							q[pro].qkey=proctab[pro].pprio;
							ready(pro, RESCHNO);
						}
					}
					else {
						pro=q[lptr->lqtail].qprev;
						lptr->ltype=READ;
						lptr->lckcnt-=proctab[currpid].proclock[*c];
						proctab[currpid].proclock[*c]=0;
						while ( pro < NPROC ) {
							lptr->lproc[pro]=proctab[pro].proclock[*c];
							lptr->lckcnt+=proctab[pro].proclock[*c];
							dequeue(pro);
							q[pro].qkey=proctab[pro].pprio;
							ready(pro, RESCHNO);
							pro=q[lptr->lqtail].qprev;
						}
						status[numlocks] = 0;
					}
				}
				else {
					lptr->lckcnt-=proctab[currpid].proclock[*c];
					proctab[currpid].proclock[*c]=0;
					lptr->lproc[currpid]=0;
					lptr->ltype=0;
					status[numlocks] = 0;
				}
			}
			else  {				
				lptr->lckcnt-=proctab[currpid].proclock[*c];
				proctab[currpid].proclock[*c]=0;
				lptr->lproc[currpid]=0;
				status[numlocks] = 0;
			}
			c--;
			struct lentry *llptr;
			int z, max_lock_prio=0;
			for (z=0; z < NLOCKS; z++) {
				llptr=&locks[z];
				if ( proctab[currpid].proclock[z] !=0 ) {
					if ( max_lock_prio < llptr->lprio ) {
						proctab[currpid].pprio=llptr->lprio;
						max_lock_prio=llptr->lprio;
					}
				}

			}
		}
	resched();
	}
	for(; numlocks > 0 ; numlocks--) {
		d=d+status[numlocks];
	}
	if (d > 0) {
		restore(ps);
		return(SYSERR);
	}
	else {
		restore(ps);
		return(OK);
	}
}
