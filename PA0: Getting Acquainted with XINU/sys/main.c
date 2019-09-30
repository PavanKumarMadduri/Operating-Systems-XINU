/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");
	printsegaddress();
	printtos();
	printprocstks(-2);
	long k;
	k=zfunction(0xaabbc0dd);
	printf("Value:%08x\n", k);
	syscallsummary_start();
	sleep(2);
	syscallsummary_stop();
	sleep(3);
	printsyscallsummary();
	return 0;
}
