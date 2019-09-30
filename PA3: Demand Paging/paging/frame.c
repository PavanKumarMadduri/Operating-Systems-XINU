/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>
fr_map_t frm_tab[NFRAMES];
/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */
//extern fr_map_t frm_tab[NFRAMES];
SYSCALL init_frm()
{
//  kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int i;
	for (i=0;i<NFRAMES;i++) {
		frm_tab[i].fr_status=FRM_UNMAPPED;
		frm_tab[i].fr_pid=BADPID;
		frm_tab[i].fr_vpno=0;
		frm_tab[i].fr_refcnt=0;
		frm_tab[i].fr_type=-1;
		frm_tab[i].fr_dirty=-1;
	}
	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */
SYSCALL get_frm(int* avail)
{
//  kprintf("To be implemented!\n");
	STATWORD ps;
	disable(ps);
	int i;
	for (i=0;i<NFRAMES;i++) {
		if (frm_tab[i].fr_status==FRM_UNMAPPED) {
			*avail=i;
			restore(ps);
			return OK;
		}
	}
	restore(ps);
	return OK;
}

/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */
SYSCALL free_frm(int i)
{

//  kprintf("To be implemented!\n");
}



