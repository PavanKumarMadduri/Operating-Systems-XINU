#include <stdio.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>
int globalpages[4];
extern fr_map_t frm_tab[NFRAMES];
void frame_status_change(int frame,int status, int pid, int vpno, int refcnt, int type, int dirty) {
	frm_tab[frame].fr_status=status;
	frm_tab[frame].fr_pid=pid;
	frm_tab[frame].fr_vpno=vpno;
	frm_tab[frame].fr_refcnt=refcnt;
	frm_tab[frame].fr_type=type;
	frm_tab[frame].fr_dirty=dirty;

	return;
}
void modify_page_table(pt_t *pt, int user, int pwt, int pcd, int acc, int dirty, int write, int mbz, int global, int avail,int base, int pres) {
	pt->pt_user = user;
	pt->pt_pwt = pwt;
	pt->pt_pcd = pcd;
	pt->pt_acc = acc;
	pt->pt_dirty = dirty;
	pt->pt_write = write;
	pt->pt_mbz = mbz;
	pt->pt_global = global;
	pt->pt_avail = avail;
	pt->pt_base = base;
	pt->pt_pres = pres;
	
	return;
}

int page_mapping(int page_type, int pid) {
	STATWORD ps;
	disable(ps);
	int page_frame,table_entry,frame_entry;
	if(get_frm(&page_frame)==SYSERR) {
		restore(ps);
		return SYSERR;
	}
	proctab[pid].pdbr = (FRAME0 + page_frame)*NBPG;
	if (page_type==DIRECTORY) {
		frame_status_change(page_frame,FRM_MAPPED, pid, -1, 0, FR_DIR, NOTDIRTY);
		for(frame_entry=0;frame_entry<NFRAMES;frame_entry++) {
			pd_t *page_directory=proctab[pid].pdbr + (frame_entry * sizeof(pd_t));
			page_directory->pd_user=0;
			page_directory->pd_pwt=0;
			page_directory->pd_pcd=0;
			page_directory->pd_acc=0;
			page_directory->pd_mbz=0;
			page_directory->pd_fmb=0;
			page_directory->pd_global=0;
			page_directory->pd_avail=0;
			page_directory->pd_pres=0;
       			page_directory->pd_write=0;
       			page_directory->pd_base=0;
			if( frame_entry < 4) {
				page_directory->pd_pres=1;
				page_directory->pd_write=1;
				page_directory->pd_base=globalpages[frame_entry];
			}
		}
		return page_frame;
	}
	else {
		frame_status_change(page_frame,FRM_MAPPED, pid, -1, 0, FR_TBL, NOTDIRTY);
		for (table_entry = 0; table_entry < 1024; table_entry++) {
			pt_t *pt = proctab[pid].pdbr + (table_entry * sizeof(pt_t));
			modify_page_table(pt,0,0,0,0,NOTDIRTY,1,0,0,0,0,0);
		}
		return page_frame;
	}
}
int init_global_pages() {
	int i,j;
	for (i=0;i<4;i++) {
		int fr=page_mapping(TABLE, NULLPROC);
		if(fr==SYSERR) {
			return SYSERR;
		}
		globalpages[i]=FRAME0 + fr;
		for (j=0;j<1024;j++) {
			pt_t *pt=globalpages[i]*NBPG+j*sizeof(pt_t);
			modify_page_table(pt,0,0,0,0,NOTDIRTY,1,0,0,0,j+i*FRAME0,1);
			frm_tab[fr].fr_refcnt++;
		}
	}
}
