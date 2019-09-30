/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{
	STATWORD ps;
	disable(ps);
	int i;
	for (i=0;i<=15;i++) {
		bsm_tab[i].bs_status=BSM_UNMAPPED;
		int p;
		for (p=0;p<=NPROC;p++) {
			bsm_tab[i].bs_pid[p]=0;
		}
		bsm_tab[i].bs_vpno=0;
		bsm_tab[i].bs_npages=0;
		bsm_tab[i].bs_privateheap=0;
		bsm_tab[i].bs_proc_count=0;
	}
	restore(ps);
	return(OK);
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{
	STATWORD ps;
	disable(ps);
	if(avail) {
		int i;
		for (i=0;i<=15;i++) {
			if (bsm_tab[i].bs_status==BSM_UNMAPPED) {
				*avail=i;
				restore(ps);
				return(OK);
			}
		}
	}
	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{
	STATWORD ps;
	disable(ps);
	if( isbadbid(i)) {
		restore(ps);
		return SYSERR;
	}
	bsm_tab[i].bs_status=BSM_UNMAPPED;
	int p;
	for (p=0;p<=15;p++) {
		bsm_tab[i].bs_pid[p]=0;
	}
	bsm_tab[i].bs_vpno=0;
	bsm_tab[i].bs_npages=0;
	bsm_tab[i].bs_privateheap=0;
	bsm_tab[i].bs_proc_count=0;

	restore(ps);
	return(OK);
}

/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{
	STATWORD ps;
	disable(ps);
	if(isbadpid(pid) || store==NULL) {
		restore(ps);
		return SYSERR;
	}
	int page_no=vaddr >> 12;
	int i;
	for (i=0;i<16;i++) {
		if (bsm_tab[i].bs_pid[pid]==1 && page_no >= bsm_tab[i].bs_vpno) {
			*pageth=page_no-bsm_tab[i].bs_vpno;
			*store=i;
			restore(ps);
			return OK;
		}
	}
	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{
	STATWORD ps;
	disable(ps);
	if (isbadpid(pid) || isbadbid(source) || isbadpagecount(npages)) {
		restore(ps);
		kprintf("BSM-MAP-1-%d\n", pid);
		return SYSERR;
	}
	proctab[pid].store[source]=1;
	proctab[pid].vhpno=4096;
	bsm_tab[source].bs_status=BSM_MAPPED;
	bsm_tab[source].bs_pid[pid]=1;
	bsm_tab[source].bs_vpno=vpno;
	bsm_tab[source].bs_npages=npages;
	bsm_tab[source].bs_proc_count++;
	kprintf("MAp count - %d\n", bsm_tab[source].bs_proc_count);
	restore(ps);
	kprintf("BSM-MAP-2\n");
	return OK;
}

/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{
	STATWORD ps;
	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		kprintf("unmap-5\n");
		return SYSERR;
	}
	
	int bid, pageth;
	if (bsm_lookup(pid, vpno*NBPG, &bid, &pageth)==SYSERR) {
		kprintf("unmap-4\n");
		restore(ps);
		return SYSERR;
	}
	if (bsm_tab[bid].bs_status==BSM_UNMAPPED) {
		restore(ps);
		kprintf("unmap-111\n");
		return SYSERR;
	}
	if (bsm_tab[bid].bs_privateheap==BSM_PRIVATE) {
                kprintf("unmap-7\n");
		if (free_bsm(bid)==SYSERR) {
			kprintf("unmap-8\n");
			restore(ps);
			return SYSERR;
		}
                restore(ps);
		kprintf("unmap-10\n");
                return SYSERR;
        }
	else {
		bsm_tab[bid].bs_pid[pid]=0;
//		proctab[pid].store[bid]=0;
		kprintf("BS ID - %d\n", bid);
		kprintf("Process Count- %d\n", bsm_tab[bid].bs_proc_count);
		restore(ps);
		kprintf("unmap-2\n");
		return OK;
	}
	restore(ps);
	kprintf("unmap-1\n");
	return SYSERR;
}
