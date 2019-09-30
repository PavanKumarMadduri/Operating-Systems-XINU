#include <stdio.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
int c=0, sp=0; 
extern struct pentry proctab[NPROC];
void printprocstks(int priority) {
	printf("void printprocstks\(int priority\)\n");
	for (c=0; c < NPROC; c++) {
		if (priority < proctab[c].pprio) {
			printf("Process [%s]\n", proctab[c].pname);
			printf("\tpid: %d\n",c);
			printf("\tpriority: %u\n", proctab[c].pprio);
			printf("\tbase: 0x%08x\n", proctab[c].pbase);
			printf("\tlimit: 0x%08x\n", proctab[c].plimit);
			printf("\tlen: %d\n", proctab[c].pstklen);
			if (c==currpid) {
				asm("movl %esp, sp"); 
				printf("\tpointer: 0x%08x\n", sp);
				}
			else
				printf("\tpointer: 0x%08x\n", proctab[c].pesp);
		}
	}
}
