#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) {
  /* release the backing store with ID bs_id */
//    kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	if ( isbadbid(bs_id) || bsm_tab[bs_id].bs_status==BSM_UNMAPPED ) {
		restore(ps);
		kprintf("Release 5\n");
		return SYSERR;
	}
	if (bsm_tab[bs_id].bs_privateheap==BSM_PRIVATE) {
		if(free_bsm(bs_id)==SYSERR) {
			restore(ps);
			kprintf("Release 4\n");
			return SYSERR;
		}
		else {
			proctab[currpid].store[bs_id]=0;
			restore(ps);
			kprintf("Release 3\n");
			return OK;
		}
	}
	else if(proctab[currpid].store[bs_id]==1){
		bsm_tab[bs_id].bs_pid[currpid]=0;
		bsm_tab[bs_id].bs_proc_count--;
		proctab[currpid].store[bs_id]=0;
		if(bsm_tab[bs_id].bs_proc_count==0) {
			if(free_bsm(bs_id)==SYSERR) {
				restore(ps);
				kprintf("Release 2\n");
				return SYSERR;
			}
			else {
				bsm_tab[bs_id].bs_vpno=0;
				bsm_tab[bs_id].bs_npages=0;
				restore(ps);
				kprintf("Release 1\n");
				return OK;
			}
		}
		restore(ps);
		kprintf("Release 0\n");
		return OK;
	}
	restore(ps);
	kprintf("Release 99\n");
	return SYSERR;
}
