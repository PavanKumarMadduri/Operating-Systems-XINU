#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

void syscallsummary_start() {
	start_flag=1;
	int i,j;
        for (i=0; i<NPROC; i++) {
                for (j=0; j<27; j++) {
                        proc_timer[i][j].ctr=0;
                        proc_timer[i][j].timer=0;
                }
        }
}
void syscallsummary_stop() {
	start_flag=0;
}

void printsyscallsummary() {
	printf("void printsyscallsummary\(\)\n");
	int i, j;
	for (j=0; j<27; j++) {
		if (proc_timer[currpid][j].ctr) {
			printf("Process [pid:%u]\n",currpid);
			printf("\tSyscall: %s, count: %u, average execution time: %u (ms)\n", proc_timer[currpid][j].name, proc_timer[currpid][j].ctr,proc_timer[currpid][j].timer/proc_timer[currpid][j].ctr);
		}
	}

}
