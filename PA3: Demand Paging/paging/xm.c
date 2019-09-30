/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{
 // kprintf("xmmap - to be implemented!\n");
	STATWORD ps;
	disable(ps);
	if(isbadbid(source) || isbadpagecount(npages) || bsm_tab[source].bs_privateheap==BSM_PRIVATE ) {	
		kprintf("xmap1\n");
		restore(ps);
		return SYSERR;
	}
	if (bsm_tab[source].bs_status==BSM_MAPPED) {
		if (npages > bsm_tab[source].bs_npages || bsm_map(currpid, bsm_tab[source].bs_vpno, source, bsm_tab[source].bs_npages)==SYSERR) {
			restore(ps);
			kprintf("xmap2\n");
			return SYSERR;
		}
		else {
			restore(ps);
			kprintf("xmap4\n");
			return OK;
		}
	}
	else {
		if (bsm_map(currpid, virtpage, source, npages)==SYSERR) {
			restore(ps);
			kprintf("xmap3\n");
			return SYSERR;
		}
		else {
                        restore(ps);
                        kprintf("xmap5\n");
                        return OK;
                }
	}

}

/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage)
{
//  kprintf("To be implemented!");
	kprintf("Xunmap called");
	STATWORD ps;
	disable(ps);
	if(virtpage < 4096) {
		restore(ps);
		kprintf("xum-1\n");
		return SYSERR;
	}
	if(bsm_unmap(currpid, virtpage, OK) == SYSERR) {
		restore(ps);
		kprintf("xum-2\n");
		return SYSERR;
	}
	restore(ps);
	kprintf("xum-3\n");
	return OK;
}
