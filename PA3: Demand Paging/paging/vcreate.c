/* vcreate.c - vcreate */
    
#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <paging.h>

/*
static unsigned long esp;
*/

LOCAL	newpid();
/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL vcreate(procaddr,ssize,hsize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	hsize;			/* virtual heap size in pages	*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	long	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{

//	kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int source_bsd,pid,source;
	if (hsize<1 || hsize>128) {
		restore(ps);
		return SYSERR;
	}
	pid=create(procaddr,ssize,priority,name,nargs,args);
	if (pid==SYSERR) {
		restore(ps);
		return SYSERR;
	}
	source=get_bsm(&source_bsd);
	if (source==SYSERR) {
		restore(ps);
		return SYSERR;
	}
	if (bsm_map(pid,4096,source, hsize)==SYSERR) {
		restore(ps);
		return SYSERR;
	}

	bsm_tab[source].bs_privateheap=BSM_PRIVATE;
//	proctab[pid].store[source]=1;
//	proctab[pid].vhpno=4096;
	proctab[pid].vhpnpages=hsize;
	proctab[pid].vmemlist->mlen=hsize*NBPG;
	proctab[pid].vmemlist->mnext = (struct mblock *)(4096 * NBPG);
	restore(ps);
	return pid;
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
