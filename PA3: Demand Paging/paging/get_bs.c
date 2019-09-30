#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

bs_map_t bsm_tab[16];
int get_bs(bsd_t bs_id, unsigned int npages) {
  /* requests a new mapping of npages with ID map_id */
	if ( isbadpagecount(npages) ||  isbadbid(bs_id) ) {
		kprintf("Get-BS-1\n");
		return SYSERR;
	}
	if (bsm_tab[bs_id].bs_status==BSM_UNMAPPED) {
		kprintf("Get-BS-3-%d\n", npages);
		return npages;
	}
	else {
		if (bsm_tab[bs_id].bs_privateheap==BSM_PRIVATE) {
			kprintf("Get-BS-5-%d\n", npages);
			return SYSERR;
		}
		else {
			kprintf("Get-BS-7-%d\n", bsm_tab[bs_id].bs_npages);
			return bsm_tab[bs_id].bs_npages;
		}
	}


}
