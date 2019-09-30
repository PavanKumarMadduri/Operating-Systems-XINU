#include <conf.h>
#include <kernel.h>
#include <lock.h>
#include <q.h>
#include <stdio.h>

struct  lentry  locks[NLOCKS];
int     nextlock;

int lcreate (void) {

	STATWORD ps;
	int     lck;

	disable(ps);
	if ( (lck=newlock())==SYSERR ) {
		restore(ps);
		return(SYSERR);
	}
	/* lqhead and lqtail were initialized at system startup */
	restore(ps);
	return(lck);

}

int newlock() {
	int     lck;
	int     i;

	for (i=0 ; i<NLOCKS ; i++) {
		lck=nextlock--;
		if (nextlock < 0) {
			nextlock = NLOCKS-1;
		}
		if (locks[lck].lstate==LFREE) {
			locks[lck].lstate = LUSED;
			return(lck);
		}
	}
	return(SYSERR);
}
